# TotallyNotLostVikingsGame — Code Analysis

**Date:** 2026-05-18
**Scope:** C++ / SDL2 / SDL_image / SDL_ttf / nlohmann::json side-scroller using an ECS-ish architecture (component maps keyed by entity id, plus per-system id lists).

This document lists findings from a static review of the sources under `src/`. Issues are grouped into:

1. Memory & resource leaks
2. Logic bugs / correctness
3. Performance & inefficiency
4. Architectural / maintainability concerns
5. Positive observations
6. Suggested remediation order

No build was attempted; all findings come from reading the code.

---

## 1. Memory & Resource Leaks

### 1.1 SDL textures are never destroyed (confirmed leak)

Files: `src/texturemanager.hpp`, `src/texturemanager.cpp`, `src/scene/scene.cpp`, `src/system.cpp`.

```cpp
// texturemanager.hpp
struct TextureManager {
  inline static std::vector<SDL_Texture *> textures;
  static void loadTextures();
  static SDL_Texture *loadTexture(const char *fileName);
};
```

`loadTextures()` calls `SDL_CreateTextureFromSurface` indirectly (via `loadTexture`) and pushes the raw pointers into a static vector. There is **no** `SDL_DestroyTexture` call anywhere in the project, and `System::clean()` does not consult `TextureManager` at all.

Worse: `Scene::init()` calls `TextureManager::loadTextures()` every time a scene is created, and `main()` re-creates `Scene` in an outer loop:

```cpp
// main.cpp
while (system.running()) {
  Scene scene;
  if (!scene.init()) { ... }
  while (Scene::isRunning) { scene.update(); scene.render(); }
}
```

So every soft restart (e.g. pressing F1 which only sets `Scene::isRunning = false`) appends **another** full set of textures into `TextureManager::textures` without freeing the previous ones. Existing render code indexes into `textures` by a hardcoded small integer (0–3), so the leaked ones are simply orphaned.

**Fix:**

- Add `TextureManager::clean()` that calls `SDL_DestroyTexture` on each non-null entry and clears the vector.
- Call it before `SDL_DestroyRenderer` in `System::clean()`.
- Either load textures **once** at program start (not in `Scene::init`), or call `clean()` before each reload.

```cpp
void TextureManager::clean() {
  for (SDL_Texture *t : textures) {
    if (t) SDL_DestroyTexture(t);
  }
  textures.clear();
}
```

### 1.2 SDL shutdown order is wrong

File: `src/system.cpp`.

```cpp
void System::clean() const {
  TTF_CloseFont(gFont);
  TTF_Quit();
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);     // renderer destroyed AFTER its window
  SDL_Quit();
}
```

Issues:

- The renderer is associated with the window; SDL2 expects `SDL_DestroyRenderer` **before** `SDL_DestroyWindow` (otherwise the renderer is silently invalidated by the window destruction).
- `IMG_Init`/`IMG_Quit` are never called even though `SDL_image` is used (works in practice, but recommended).
- If `gFont` is `nullptr` (font failed to load), `TTF_CloseFont(nullptr)` is a no-op in modern SDL_ttf but still relies on that — fine, but the code never *checks* the load.
- Texture cleanup (1.1) is missing here.

**Fix (sketch):**

```cpp
void System::clean() const {
  TextureManager::clean();
  if (gFont)    { TTF_CloseFont(gFont); gFont = nullptr; }
  if (renderer) { SDL_DestroyRenderer(renderer); renderer = nullptr; }
  if (window)   { SDL_DestroyWindow(window);     window   = nullptr; }
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}
```

### 1.3 `loadTexture` may push null pointers

File: `src/texturemanager.cpp`.

```cpp
SDL_Texture *TextureManager::loadTexture(const char *fileName) {
  SDL_Surface *tempSurface = IMG_Load(fileName);
  if (tempSurface != nullptr) {
    SDL_SetColorKey(...);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(System::renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    return tex;
  }
  std::cout << "ERROR: " << SDL_GetError() << std::endl;
  return nullptr;
}
```

- `SDL_CreateTextureFromSurface` failure is not checked (could still return `nullptr`).
- `loadTextures()` blindly `emplace_back`s the result. Later, `RenderSystem::renderStatic/renderNonStatic` calls `SDL_RenderCopyEx(..., textures.at(index), ...)` with a possibly null texture.
- An asset filename typo would silently produce a black/garbled frame instead of a clear error and shift all subsequent indices.

**Fix:** Check the result, log a meaningful error including the filename, and either abort or insert a placeholder texture so indices remain stable.

### 1.4 `SDL_RWops` is closed only on the success path

File: `src/scene/scenecreator.cpp` — `createLevel` calls `SDL_RWclose(file);` only after the parse loop completes successfully. In `Scene::init()` the file is checked for non-null before being handed off, but if `SceneCreator::createLevel` is ever changed to early-return (e.g. on parse error), the handle would leak. Not a current bug, but fragile.

**Fix:** Wrap the `SDL_RWops*` in a RAII helper (`std::unique_ptr<SDL_RWops, decltype(&SDL_RWclose)>`).

### 1.5 `GameData::init()` does not check `ifstream` or catch JSON exceptions

File: `src/gamedata.cpp`.

```cpp
json GameData::init() {
  std::ifstream f("example.json");
  json data = json::parse(f);     // throws on missing/invalid file
  json asd  = data.at("player");  // throws if key missing
  return asd;
}
```

Not currently called from runtime code paths (the call site in `SceneCreator::getGameData()` is dead), but if reactivated it will throw uncaught exceptions and tear down the program.

**Fix:** Check `f.is_open()`, wrap in `try/catch (const nlohmann::json::exception&)`, return `std::optional<json>` or a default.

### 1.6 Static state across scene restarts

`SceneCreator::getId()` uses a function-local `static int id = 2;` that never resets. Combined with `Scene` being a stack object re-created in `main`, every reload of the level produces ever-larger entity ids. This is not a leak per se, but combined with 1.1 it strongly hints at "soft restart" not being clean.

**Fix:** Make the id counter a member of `SceneInfo` (or pass it in), and reset on scene creation.

---

## 2. Logic Bugs / Correctness

### 2.1 `ControlSystem::handleInput` checks the wrong axis on left input

File: `src/systems/controlsystem.cpp` (lines ~184–190).

```cpp
if (inputL && !inputR) {
  if (transformComponent.velocity.y > 0) {   // <-- should be velocity.x
    transformSystem->stopX(transformComponent);
  }
  transformSystem->moveLeft(transformComponent);
  noInput = false;
}
```

The right-input branch correctly checks `velocity.x < 0`. The left branch checks `velocity.y > 0`, which means "falling" rather than "moving right". The intended snappy direction reversal therefore does not happen when switching from right to left while grounded.

**Fix:** Change to `transformComponent.velocity.x > 0`.

### 2.2 Missing `break` in `ControlSystem::determineControlState` switch (fall-through bug)

File: `src/systems/controlsystem.cpp` (lines ~46–93).

```cpp
switch (collisionComponent.collisionTileDown) {
case TILE::NONE:    ...; break;
case TILE::SOLID:
  ...
  } else if (controlComponent.controlState == CONTROL_STATE::ATTACKING) {
    attackComponent.attacking = true;
    attackComponent.currentAttackFrames++;
    break;     // <-- only breaks out of the `if`, not the `switch`? Actually breaks the switch — but the outer SOLID case has no unconditional break
  }
  // FALLS THROUGH into SEMISOLID
case TILE::SEMISOLID: break;
case TILE::LADDER:    break;
case TILE::LADDERSTART: ...
```

The `TILE::SOLID` case has no terminating `break` after the conditional block. When `controlState != ATTACKING`, execution falls through into `TILE::SEMISOLID` (harmless empty body) — but a future change to `SEMISOLID` will silently misbehave. This is a maintenance bomb.

**Fix:** Add an explicit `break;` at the end of the `TILE::SOLID` case (or refactor to `if/else` since the cases are not really independent label targets).

### 2.3 Hardcoded player entity id = 1

Files: `src/scene/scenecreator.cpp` (`createPC` always uses `id = 1`), `src/utilities/constants.hpp` (`PLAYER_INDEX = 1`), `src/systems/attacksystem.cpp` (`...at(1)` for player), `src/systems/aisystem.cpp` (`...at(1)` for player), `src/scene/scene.cpp`.

Consequences:

- Two `PC` cells in the CSV would overwrite each other in every component map, **and** add duplicate `1`s to the id vectors (`transformIds`, `controlIds`, `healthIds`, …). Systems iterating those vectors would process the same entity twice.
- `ControlSystem::changePlayerCharacter` cycles `selectedPlayerIndex` through 0,1,2, but id 0 is the UI element and id 2 likely does not exist (`getId()` starts at static 2 and pre-increments, so the first generated id is 3). This will throw `std::out_of_range` from `unordered_map::at`.

**Fix:** Generate the PC id via `getId()`, store player ids in a dedicated `std::vector<int> playerIds` in `SceneInfo`, and reference players through that.

### 2.4 `AnimationSystem::previousAnimationState` is shared across entities

File: `src/systems/animationsystem.hpp`, `src/systems/animationsystem.cpp`.

```cpp
class AnimationSystem {
  ANIMATION_STATE previousAnimationState = ANIMATION_STATE::STILL;
  ...
};
```

`determineAnimationState`/`determineAIAnimationState` assign `previousAnimationState = animationComponent.animationState;` before computing the new state, and `resetAnimation` uses it to reset `animationFrame`. Because there is exactly **one** `previousAnimationState` for the whole system, processing entity B clobbers the previous state from entity A. With multiple animated entities, animation frames will reset incorrectly or not at all.

**Fix:** Move `previousAnimationState` into `AnimationComponent`.

### 2.5 `AISystem` reads `facingRight` before it is updated

File: `src/systems/aisystem.cpp` (lines ~12–46).

```cpp
bool facingRight = currentLevel.transformComponents.at(id)->facingRight;
...
if (facingRight) { velx = 4;  attackRect = &attackComponent.rightCollisionRect; }
else             { velx = -4; attackRect = &attackComponent.leftCollisionRect;  }

if (collisionComponent.collisionTileRight == TILE::SOLID ||
    collisionComponent.collisionTileDown  == TILE::NONE) {
  velx = -4;                                  // changes direction…
} else if (collisionComponent.collisionTileLeft == TILE::SOLID ||
           collisionComponent.collisionTileDown == TILE::NONE) { // unreachable
  velx = 4;
}
```

Problems:

- `facingRight` is the value from the *previous* frame, so `attackRect` is associated with the old direction even if `velx` is flipped on this frame.
- The `else if` condition repeats `collisionTileDown == TILE::NONE`, which already matched the `if`. The "edge of platform on the left → turn around" branch is dead code.
- `velx` is also unconditionally set every frame from facing direction, meaning enemies cannot truly stop (e.g. while attacking they get `velx = 0` in the ATTACKING case, but `determineAIState` may have just moved them back to WALKING, immediately re-applying ±4).

**Fix:** Compute desired direction first, write `velx`, derive `facingRight` from the sign of `velx`, then pick `attackRect`. Replace the duplicated condition with `collisionTileLeft == TILE::SOLID` only.

### 2.6 `Physics::attackCollision` is not an AABB test

File: `src/utilities/physics.cpp`.

```cpp
bool attackCollision(SDL_Rect const &player, SDL_Rect const &obj) {
  Velocity playerVelocity = {0, 0};
  return collisionDown(player, obj, playerVelocity) || ...;
}
```

The `collisionDown/Up/Left/Right` helpers are **swept** checks; with zero velocity they short-circuit on boundary conditions (e.g. `player.y + player.h <= obj.y + obj.h` and `player.y + player.h + 0 >= obj.y`). For two rectangles deeply overlapping but oriented "wrong" relative to the chosen helper, every directional check can return `false`, producing false negatives for hits.

**Fix:**

```cpp
bool attackCollision(SDL_Rect const &a, SDL_Rect const &b) {
  return checkCollision(a, b);
}
```

### 2.7 `CollisionComponent::center` default initializer is wrong

File: `src/components/collisioncomponent.hpp`.

```cpp
Utils::Position center = {(collisionRect.x + collisionRect.w + collisionRect.w) / 2,
                          (collisionRect.y + collisionRect.h + collisionRect.h) / 2};
```

This computes `(x + 2w)/2`, not the rectangle center `x + w/2`. Additionally, default member initializers for `center` reference `collisionRect`, which is itself default-initialized to `{0,0,0,0}` at that point (in-class member init order: `collisionRect` then `center`, both with NSDMIs — actually evaluated in declaration order, so this is at least defined, but the value is meaningless because the constructor body subsequently assigns `collisionRect = collision`, leaving `center` stale at `{0,0}`). `CollisionDetectionSystem::updateCollisionRect` recomputes it later (also with the same wrong formula).

**Fix:**

```cpp
center = { collisionRect.x + collisionRect.w / 2,
           collisionRect.y + collisionRect.h / 2 };
```

Set in the constructor body and in `updateCollisionRect`.

### 2.8 `TransformSystem::update` updates `tilePosition` from the *old* position

File: `src/systems/transformsystem.cpp`.

```cpp
const int xpos = transformComponent.position.x;
...
transformComponent.position.x = xpos + xvel;
if (transformComponent.tilePosition.x != xpos / 128) {   // uses old xpos
  transformComponent.tilePosition.x = xpos / 128;
}
```

Same for Y. So `tilePosition` always lags the actual position by one frame. Anything keying off tile position (currently nothing critical, but it's exposed) will be off-by-one.

**Fix:** Use `transformComponent.position.x / 128` after writing the new position.

### 2.9 `TransformSystem::moveLeft` can exceed `playerSpeedMax`

```cpp
void TransformSystem::moveLeft(TransformComponent &t) const {
  if (t.velocity.x < -playerSpeedMax) {
    t.velocity.x = -playerSpeedMax;
  } else {
    t.velocity.x = t.velocity.x - playerSpeed; // can overshoot, e.g. -7.9 - 0.4 = -8.3
  }
}
```

`moveRight` does the correct "look-ahead" check (`velocity.x + playerSpeed > playerSpeedMax`). `moveLeft` only catches the already-out-of-range case, so velocity can transiently exceed the cap.

**Fix:** Mirror `moveRight`:

```cpp
if (t.velocity.x - playerSpeed < -playerSpeedMax) t.velocity.x = -playerSpeedMax;
else                                              t.velocity.x -= playerSpeed;
```

### 2.10 Integer position truncates fractional velocity

`Utils::Position` is `int`, `Utils::Velocity` is `float`. `position.x = xpos + xvel` (int + float → float, then implicit truncation to int). Small velocities (e.g. gravity step 0.75 added to a player with `velocity.y` between cycles) accumulate predictably only because the velocity itself is float; positions snap to integer pixels each frame, causing visible jitter and making sub-pixel acceleration impossible. Climbing speed cap is `4.0`, integer-friendly, so this is partly hidden — but the gravity unit (0.75) and `playerSpeed` (0.4) are not integers.

**Fix:** Either store position as `float` and convert to `int` only for rendering/collision, or accumulate a `subPixel` offset.

### 2.11 Input edge handling for jump/attack is fragile

File: `src/systems/controlsystem.cpp`.

```cpp
if (keyboardStateArray[SDL_SCANCODE_D])  { inputJump = true; }
if (!keyboardStateArray[SDL_SCANCODE_D]) { canJump   = true; }   // released → re-armed
```

`canJump`/`canAttack` are only re-armed when the key is *not* held. Combined with the once-per-frame `SDL_PollEvent` (see 3.3), brief taps can still register, but the semantics are unusual: you must fully release before re-triggering. This is probably intended (no auto-fire) but it would be clearer to track edge transitions explicitly. Also `inputChangeCharacter` is checked but never set anywhere — dead code path.

### 2.12 `Scene::isRunning` and `System::isRunning` are static booleans

`Scene::isRunning` is `inline static bool` on the `Scene` struct, set/reset from `ControlSystem` via direct access. `System::isRunning` likewise. Static state on classes that have instance lifetime semantics (a `Scene` is created per iteration in `main`) is asking for trouble — the static flag persists across scene instances and is the only reason the soft-restart works at all.

**Fix:** Make them instance members and pass references where needed.

### 2.13 Frame-pacing has a bug when over budget

File: `src/main.cpp`.

```cpp
Uint64 ticksEnd = SDL_GetTicks64() - ticksStart;
if (Constants::FRAMEDELAY > ticksEnd) {
  SDL_Delay(Constants::FRAMEDELAY - ticksEnd);
  continue;
}
```

If a frame takes *less* than the budget, we delay and `continue` — fine. If it takes *more*, we fall out of the loop body and immediately start the next frame, but there is no `delta`/catch-up logic anywhere. All movement is in "pixels per frame", so frame drops translate directly to slowdown rather than skipped motion. Not strictly a bug, but worth knowing.

### 2.14 Scene CSV parser drops the last cell and handles CRLF poorly

File: `src/scene/scenecreator.cpp`.

```cpp
while (SDL_RWread(file, &c, 1, 1) == 1) {
  if (c == '\n') { y++; x = 0; continue; }   // does not flush currentValue
  if (c == ',')  { ...flush... continue; }
  currentValue += c;
}
```

- A trailing cell without a comma before the newline (or before EOF) is silently dropped.
- On Windows-line-ended files, `\r` ends up appended to `currentValue`, so e.g. `"PC\r"` no longer matches `starts_with("PC")` only because `starts_with` checks the prefix — it would still match `"PC"` prefix, but `"E\r"` then matches `starts_with("E")` and gets created at the wrong cell. Worse, an empty cell at the end of a `\r\n` line becomes `"\r"`, which falls into the `else` branch and logs "is unknown".

**Fix:** Flush on both `,` and `\n`/EOF; strip `\r` before matching; or use `std::getline` + comma split.

### 2.15 `CollisionDetectionSystem::checkPlayer*Collision` mutates `xposDif`/`yposDif` on every hit

File: `src/systems/collisiondetectionsystem.cpp`. The loops over `staticCollisionIds` overwrite `xposDif`/`yposDif` each time a side collides, so if multiple tiles collide on the same side in one frame, only the **last** tile's offset survives. Combined with the unsorted iteration order of `unordered_map`-backed ids (actually iterated via the `vector<int> staticCollisionIds` in insertion order — so deterministic, but still dependent on level layout), the chosen depenetration may not be the smallest. This produces sticky walls or skipped frames at tile seams.

**Fix:** Track the maximum overlap per side and apply that; or process the smallest depenetration vector.

### 2.16 `Camera::viewPort.w/h` are repurposed as right/bottom edges

Files: `src/camera.cpp`, `src/systems/rendersystem.cpp`.

```cpp
viewPort = {offset.x, offset.y, SCREENWIDTH + offset.x, SCREENHEIGHT + offset.y};
...
return ... && destinationRect.x <= Camera::viewPort.w &&
             destinationRect.y <= Camera::viewPort.h;
```

`SDL_Rect::w/h` are conventionally width/height, but here they are used as absolute right/bottom coordinates. Anything that *does* treat them as width/height (e.g. a future `SDL_RenderSetViewport(... &viewPort)`) will be wildly wrong.

**Fix:** Use a dedicated struct with `left/top/right/bottom`, or use `w`/`h` correctly and recompute edges as `x + w`/`y + h` on the fly.

### 2.17 `Camera::updateOffsetRelativeToScreen` ignores its arguments' semantics

```cpp
void Camera::updateOffsetRelativeToScreen(int offsetx, int offsety) {
  offset.x = offsetx - SCREENWIDTH  * 1 / 2;
  offset.y = offsety - SCREENHEIGHT * 1 / 2;
}
```

This is fine as "center camera on world position (offsetx, offsety)", but the parameter names are misleading. Minor.

### 2.18 `System::init` never actually fails

```cpp
if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
  TTF_Init();
  gFont = TTF_OpenFont("font.ttf", 32);
  ...
  isRunning = true;
} else {
  isRunning = false;
}
```

If `TTF_Init`, `TTF_OpenFont`, `SDL_CreateWindow`, or `SDL_CreateRenderer` fail, `isRunning` is still set to `true` and the program proceeds to use null pointers. Only outright `SDL_Init` failure is treated as fatal.

**Fix:** Check every return value and set `isRunning = false` (and log `SDL_GetError`) on any failure.

---

## 3. Performance / Inefficiency

### 3.1 Collision detection is O(N·M) every frame

`CollisionDetectionSystem::checkCollision` iterates every static collider for every dynamic collider. At 128 px tiles and a 1920×1080 screen the visible tile count alone is ~120; full levels can be much larger. With several dynamic entities this scales badly.

**Fix:** Use the tile grid — given an entity's `tilePosition`, only test the 3×3 (or 4×4 if motion is large) neighborhood. The `tileComponents` already key tiles by grid coordinates; lookups by `(x/128, y/128)` are trivially `O(1)` with an `unordered_map<pair<int,int>, int>` or a flat 2D array sized to `levelLength/128 × levelHeight/128`.

### 3.2 `HealthSystem::update` logs every entity every frame

```cpp
std::cout << "id " << id << ", current health " << ... << std::endl;
```

`std::endl` flushes; doing this for every health-bearing entity at 60 FPS is a measurable hit and floods the console. The "damage taken" value printed is always 0 because it is zeroed on the line above.

**Fix:** Only log on damage events or death. Guard behind a debug flag.

### 3.3 Event pump runs once per frame

`ControlSystem::takeInput` calls `SDL_PollEvent` exactly once, then drops out. Pending events accumulate; specifically `SDL_QUIT` may be delayed and other event types are never drained, which can cause the OS to mark the window as unresponsive.

**Fix:**

```cpp
SDL_Event e;
while (SDL_PollEvent(&e)) {
  if (e.type == SDL_QUIT) { Scene::isRunning = false; System::isRunning = false; }
}
```

### 3.4 Repeated `unordered_map::at` lookups for the same id

Every system does `*currentLevel->XComponents.at(id)` multiple times per entity. `at` is `O(1)` average but does a hash + bucket walk + bounds-checked dereference each time. For hot loops (`CollisionDetectionSystem`, `RenderSystem::update`, `AnimationSystem`) the redundant hashing is wasted work.

**Fix:** Cache references at the top of each iteration (most systems already do this; the ones that don't could). Better: switch to a sparse-set or flat array indexed by id.

### 3.5 `unique_ptr` per component → pointer chasing

Each component lives in its own heap allocation. Iterating `objectRenderIds` then dereferencing `renderComponents.at(id).get()` is a double indirection per access. For a small game this is fine; for performance work, a struct-of-arrays / sparse set would be much friendlier to the cache and avoid the allocations.

### 3.6 `RenderSystem::update` traverses every render id twice (update + render)

Two passes (`update` rebuilds `destinationRect` from `transformComponent`, `render` then issues `SDL_RenderCopyEx`) iterate the same id lists. Could be merged, but readability cost is real — minor.

### 3.7 `Constants::FRAMEDELAY` uses integer division of `1000/60 = 16`

The actual frame budget is 16.666… ms, but the code caps at 16 ms — so frames are systematically slightly too fast. Combined with `SDL_RENDERER_PRESENTVSYNC` you'll usually get vsync-pacing anyway, but the sleep loop is fighting it.

### 3.8 `AttackSystem::checkAttackCollisions` branches on player id == 1

It special-cases `attackerId == 1` (player attacking enemies) vs anything else (enemies attacking player at id 1). Once 2.3 (hardcoded player id) is fixed, this should naturally collapse into a single loop over `healthIds` skipping the attacker.

---

## 4. Architectural / Maintainability

### 4.1 Inconsistent component base class

`AttackComponent` and `HealthComponent` do not inherit from `IComponent`; all others do. `IComponent` only carries `ID` and `parentID`, neither of which is consistently used (most systems look up by map key, not by `parentID`). Either commit to it or drop it.

### 4.2 Static state masquerading as instance members

`System::window/renderer/gFont/isRunning`, `Scene::isRunning`, `Camera::focusArea/viewPort/offset`, `TextureManager::textures`, `SceneCreator` everything-static. There is effectively one global `System`, one global `Camera`, one global texture cache. The classes pretend to be regular types (constructors, instance methods) but mutating any instance changes the global state. This makes testing impossible and reloading scenes brittle (see 1.1, 1.6).

**Fix:** Pick a side — pure singletons / free functions, or proper instances threaded through the call graph.

### 4.3 Systems hold raw, nullable `SceneInfo*`

Every system has `SceneInfo *currentLevel = nullptr;` and is wired up manually in `Scene::init()`. Forgetting one (or adding a new system) is a silent crash waiting to happen — the assignment is by hand, not by any mechanism.

**Fix:** Pass `SceneInfo&` into `update()`, or store a reference set at construction.

### 4.4 Dead code

`src/entities/entity.{hpp,cpp}` and `src/entities/gameobject.{hpp,cpp}` are entirely commented out. CMake's `file(GLOB ...)` still picks them up — they compile to empty TUs, which is harmless but confusing. `Scene::handleEvents` is declared but never defined. `SceneCreator::data` is declared but the static is commented out (linker would complain if anyone referenced it). The commented `template <typename T> T *GetComponent(...)` in `Scene` references an old design.

**Fix:** Delete dead files; remove dead declarations.

### 4.5 `nlohmann/json` is included widely but never used

`rendercomponent.hpp` includes `<nlohmann/json.hpp>` and declares `to_json/from_json` stubs that do nothing. `scenecreator.hpp` and `gamedata.hpp` also pull it in. This significantly increases compile times for very little benefit at the moment.

**Fix:** Remove the include + stubs until serialization is actually wired up.

### 4.6 `file(GLOB ...)` in CMake

`CMakeLists.txt` uses `file(GLOB SOURCES src/*/*.cpp src/*.cpp)`. New files are not picked up until CMake is re-run, which is a well-known footgun.

**Fix:** Either list sources explicitly or use `CONFIGURE_DEPENDS`.

### 4.7 `[[no_unique_address]]` on stateful members

`Scene` annotates many of its system members with `[[no_unique_address]]`, including `ControlSystem` (which carries `inputR`, `inputL`, `canJump`, …). The attribute only has effect on empty types; on stateful ones it is a no-op and adds noise.

**Fix:** Remove the attribute or restrict it to empty systems.

### 4.8 Magic numbers everywhere

`128` (tile size), `64` (attack hitbox half-width), `8` (animation step divisor), `36` (attack frames), `4` (enemy walk speed), `0.75/11.0` (gravity), `220,0,212` (color key) are sprinkled in code. Centralize in `Constants` or per-component config.

### 4.9 `Position`/`Velocity` mismatch

See 2.10 — `Position` is `int`, `Velocity` is `float`. They are used in arithmetic together; promote consistently.

### 4.10 Header hygiene

Several headers include `<SDL2/...>` while others include `"SDL_render.h"` directly (bare). Mixed `#pragma once` + include guards is also inconsistent. Standardize.

---

## 5. Positive Observations

- Component ownership through `std::unique_ptr` in maps is clean and prevents component leaks themselves.
- `SDL_Surface` is correctly freed after creating a texture.
- `SDL_RWops` is closed on the normal success path of CSV parsing.
- Systems are decently separated (render, control, gravity, collision, AI, animation, attack, health), making targeted fixes easy.
- `RenderSystem::inView` does cheap frustum culling for world-space sprites — a nice optimization already present.
- `unique_ptr`-based components mean no manual `delete`; resetting `SceneInfo` (currently implicit on `Scene` destruction) will free them all automatically.
- VSync is enabled (`SDL_RENDERER_PRESENTVSYNC`).

---

## 6. Suggested Fix Order

In order of "biggest bug-for-effort":

1. **Texture cleanup** (1.1) — add `TextureManager::clean()`, call from `System::clean()`, and move `loadTextures()` out of `Scene::init`.
2. **SDL shutdown order** (1.2) — destroy renderer before window; add `IMG_Quit`.
3. **`ControlSystem` left-axis bug** (2.1) and **fall-through** (2.2).
4. **`AnimationSystem` per-entity previous state** (2.4).
5. **Hardcoded player id** (2.3) — introduce `playerIds` and route systems through it.
6. **`Physics::attackCollision` → AABB** (2.6) and **`CollisionComponent::center` formula** (2.7).
7. **CSV parser robustness** (2.14) and **`getId` reset** (1.6).
8. **`AISystem` direction logic** (2.5).
9. **Frame-perfect `TransformSystem` updates** (2.8/2.9) and **float positions** (2.10).
10. **Event pump loop** (3.3) and **silence per-frame logging** (3.2).
11. **Spatial-hash collision** (3.1).
12. **`System::init` error checks** (2.18).
13. **Architectural cleanup** (4.x) once gameplay is correct.

---

## Appendix A — Concrete patches

### A.1 `TextureManager::clean`

```cpp
// texturemanager.hpp
struct TextureManager {
  inline static std::vector<SDL_Texture *> textures;
  static void loadTextures();
  static SDL_Texture *loadTexture(const char *fileName);
  static void clean();
};

// texturemanager.cpp
void TextureManager::clean() {
  for (SDL_Texture *t : textures) {
    if (t) SDL_DestroyTexture(t);
  }
  textures.clear();
}
```

### A.2 `System::clean`

```cpp
void System::clean() const {
  TextureManager::clean();
  if (gFont)    { TTF_CloseFont(gFont); gFont = nullptr; }
  if (renderer) { SDL_DestroyRenderer(renderer); renderer = nullptr; }
  if (window)   { SDL_DestroyWindow(window);     window   = nullptr; }
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}
```

### A.3 `ControlSystem::handleInput` left branch

```cpp
if (inputL && !inputR) {
  if (transformComponent.velocity.x > 0) {
    transformSystem->stopX(transformComponent);
  }
  transformSystem->moveLeft(transformComponent);
  noInput = false;
}
```

### A.4 `Physics::attackCollision`

```cpp
bool attackCollision(SDL_Rect const &a, SDL_Rect const &b) {
  return checkCollision(a, b);
}
```

### A.5 `CollisionComponent::center`

```cpp
CollisionComponent(SDL_Rect collision, int id) : collisionRect(collision) {
  parentID = id;
  center   = { collisionRect.x + collisionRect.w / 2,
               collisionRect.y + collisionRect.h / 2 };
}
```

### A.6 `SDL_PollEvent` drain

```cpp
SDL_Event e;
while (SDL_PollEvent(&e)) {
  if (e.type == SDL_QUIT) {
    Scene::isRunning  = false;
    System::isRunning = false;
  }
}
```

### A.7 `TransformSystem` tile sync

```cpp
transformComponent.position.x = xpos + xvel;
transformComponent.tilePosition.x = transformComponent.position.x / 128;
```

(Symmetric for Y.)

### A.8 `AnimationSystem` per-component previous state

```cpp
// animationcomponent.hpp
struct AnimationComponent : public IComponent {
  ANIMATION_STATE animationState         = ANIMATION_STATE::STILL;
  ANIMATION_STATE previousAnimationState = ANIMATION_STATE::STILL;
  int animationFrame                     = 0;
  const int WALKING_ANIMATION_FRAMES     = 8;
};

// animationsystem.cpp
void AnimationSystem::resetAnimation(AnimationComponent &ac) const {
  if (ac.previousAnimationState != ac.animationState) ac.animationFrame = 0;
  ac.previousAnimationState = ac.animationState;
}
```

(Remove the system-level `previousAnimationState` and the `previousAnimationState = ...;` assignments inside `determine*AnimationState`.)

