# TotallyNotLostVikingsGame AI Code Analysis

Date: 2026-05-18  
Project: C++ SDL2 2D side-scroller using an Entity-Component-System style architecture.

## Summary

The project has a reasonable ECS-style separation of components and systems, and most component ownership is handled through `std::unique_ptr`, which prevents many ordinary component memory leaks. The largest confirmed resource leak is SDL texture lifetime management: textures are loaded into a static vector and never destroyed. There are also several logic issues around player IDs, input handling, animation state, AI movement, level parsing, camera bounds, and collision detection.

No IDE/compiler errors were reported for the inspected source files during analysis.

---

## High Priority Issues

### 1. SDL textures are leaked

Files:

- `src/texturemanager.hpp`
- `src/texturemanager.cpp`
- `src/scene/scene.cpp`

`TextureManager::textures` is a static vector of raw `SDL_Texture*` values:

```cpp
inline static std::vector<SDL_Texture *> textures;
```

Textures are loaded here:

```cpp
void TextureManager::loadTextures() {
  textures.emplace_back(TextureManager::loadTexture("assets/baleog.png"));
  textures.emplace_back(TextureManager::loadTexture("assets/tiles.png"));
  textures.emplace_back(TextureManager::loadTexture("assets/UI.png"));
  textures.emplace_back(TextureManager::loadTexture("assets/enemy.png"));
}
```

There is no matching `SDL_DestroyTexture()` call anywhere in the codebase.

This becomes worse because `Scene::init()` calls `TextureManager::loadTextures()` every time a scene is initialized. If the scene is restarted, another full set of textures is loaded and appended to the static vector.

Recommended fix:

- Add `TextureManager::clean()` or `TextureManager::destroyTextures()`.
- Loop through `textures`.
- Call `SDL_DestroyTexture(texture)` for every non-null texture.
- Clear the vector.
- Call the cleanup function before SDL renderer/window shutdown.

Example:

```cpp
void TextureManager::clean() {
  for (SDL_Texture *texture : textures) {
    if (texture != nullptr) {
      SDL_DestroyTexture(texture);
    }
  }
  textures.clear();
}
```

---

### 2. `System::clean()` destroys SDL objects in a risky order

File: `src/system.cpp`

Current cleanup order:

```cpp
TTF_CloseFont(gFont);
TTF_Quit();
SDL_DestroyWindow(window);
SDL_DestroyRenderer(renderer);
SDL_Quit();
```

The renderer should normally be destroyed before the window it belongs to. `IMG_Quit()` is also missing even though SDL_image is used.

Recommended order:

```cpp
TTF_CloseFont(gFont);
gFont = nullptr;

TextureManager::clean();

SDL_DestroyRenderer(renderer);
renderer = nullptr;

SDL_DestroyWindow(window);
window = nullptr;

TTF_Quit();
IMG_Quit();
SDL_Quit();
```

---

### 3. Texture loading does not robustly handle failures

File: `src/texturemanager.cpp`

Current code checks whether `IMG_Load()` succeeds, but does not check whether `SDL_CreateTextureFromSurface()` succeeds:

```cpp
SDL_Texture *tex = SDL_CreateTextureFromSurface(System::renderer, tempSurface);
SDL_FreeSurface(tempSurface);
return tex;
```

If texture creation fails, `nullptr` is pushed into `TextureManager::textures`. Later rendering passes that null pointer to `SDL_RenderCopyEx()`.

Recommended fix:

- Check `System::renderer` before creating textures.
- Check the result of `SDL_CreateTextureFromSurface()`.
- Log `SDL_GetError()` or `IMG_GetError()`.
- Avoid storing null textures.

---

### 4. Scene restart leaks textures repeatedly

Files:

- `src/main.cpp`
- `src/scene/scene.cpp`
- `src/texturemanager.cpp`

`main()` contains an outer loop:

```cpp
while (system.running()) {
  Scene scene;
  if (!scene.init()) {
    ...
  }

  while (Scene::isRunning) {
    scene.update();
    scene.render();
  }
}
```

Pressing F1 sets only:

```cpp
Scene::isRunning = false;
```

This exits the inner scene loop, but the outer application loop continues and creates a new scene. `Scene::init()` then reloads all textures. Since textures are static and never destroyed, this leaks another full set of textures each restart.

Recommended fix:

- Load global textures once during application startup, or
- Destroy scene textures before restarting, and
- Make ownership clear: either the scene owns textures or a global resource manager owns them.

---

## Logic Bugs

### 5. `ControlSystem::handleInput()` checks the wrong velocity axis when moving left

File: `src/systems/controlsystem.cpp`

Current code:

```cpp
if (inputL && !inputR) {
  if (transformComponent.velocity.y > 0) {
    transformSystem->stopX(transformComponent);
  }
  transformSystem->moveLeft(transformComponent);
  noInput = false;
}
```

This checks `velocity.y`, but the intention is almost certainly to check `velocity.x`.

Recommended fix:

```cpp
if (transformComponent.velocity.x > 0) {
  transformSystem->stopX(transformComponent);
}
```

---

### 6. `AnimationSystem` tracks previous animation state globally instead of per entity

Files:

- `src/systems/animationsystem.hpp`
- `src/systems/animationsystem.cpp`

Current member:

```cpp
ANIMATION_STATE previousAnimationState = ANIMATION_STATE::STILL;
```

This value is shared by the whole `AnimationSystem`. During `AnimationSystem::update()`, multiple entities are processed. The previous animation state from one entity can affect the next entity.

Problematic logic:

```cpp
if (previousAnimationState != animationComponent.animationState) {
  animationComponent.animationFrame = 0;
}
```

Recommended fix:

Store previous state in `AnimationComponent` instead:

```cpp
struct AnimationComponent : public IComponent {
  ANIMATION_STATE animationState = ANIMATION_STATE::STILL;
  ANIMATION_STATE previousAnimationState = ANIMATION_STATE::STILL;
  int animationFrame = 0;
  const int WALKING_ANIMATION_FRAMES = 8;
};
```

Then reset animation per component, not per system.

---

### 7. `SceneCreator::createPC()` always uses entity ID `1`

File: `src/scene/scenecreator.cpp`

Current code:

```cpp
void SceneCreator::createPC(int x, int y, SceneInfo &level) {
  int id = 1;
  ...
}
```

Problems:

- Multiple `PC` entries in the level file overwrite component maps for ID `1`.
- ID vectors such as `transformIds`, `controlIds`, `healthIds`, etc. can contain duplicate `1` values.
- Systems may process the same entity multiple times.
- This prevents multiple player characters unless the rest of the code is heavily special-cased.

Recommended fix:

- Use generated IDs for all entities.
- Store player IDs separately in `SceneInfo`, for example `std::vector<int> playerIds`.
- Store the active player ID explicitly instead of assuming `1`.

---

### 8. Several systems hardcode player ID `1`

Files:

- `src/scene/scene.cpp`
- `src/systems/attacksystem.cpp`
- `src/systems/aisystem.cpp`
- `src/utilities/constants.hpp`

Examples:

```cpp
constexpr int PLAYER_INDEX = 1;
```

```cpp
HealthComponent &healthComponent = *currentLevel->healthComponents.at(1);
```

```cpp
CollisionComponent const &player = *currentLevel.collisionComponents.at(1);
```

This makes the ECS brittle and can crash if entity `1` is missing.

Recommended fix:

- Add a `playerEntityId` or `playerIds` field to `SceneInfo`.
- Pass the active player ID into systems.
- Avoid magic entity IDs.

---

### 9. Character switching appears incomplete and unsafe

File: `src/systems/controlsystem.cpp`

`inputChangeCharacter` is checked:

```cpp
if (inputChangeCharacter) {
  changePlayerCharacter(selectedPlayerIndex);
  ...
}
```

But no key currently sets `inputChangeCharacter = true`.

Also:

```cpp
void ControlSystem::changePlayerCharacter(int &selectedPlayerIndex) const {
  selectedPlayerIndex++;
  if (selectedPlayerIndex == 3) {
    selectedPlayerIndex = 0;
  }
}
```

This cycles through IDs `0`, `1`, and `2`, but currently:

- UI uses ID `0`.
- Player uses ID `1`.
- Entity `2` may not exist, because `SceneCreator::getId()` starts at static `2`, increments, and returns `3` first.

If this feature is enabled, it can easily cause `unordered_map::at()` exceptions.

Recommended fix:

- Store valid controllable player IDs in `SceneInfo`.
- Cycle through that vector instead of hardcoded numeric IDs.

---

### 10. Level parser can skip the last cell if the CSV does not end with a comma

File: `src/scene/scenecreator.cpp`

The parser processes a cell only when it sees a comma:

```cpp
if (c == ',') {
  ...
  currentValue = "";
  continue;
}
```

If the final value in a line or file is not followed by a comma, it may never be processed.

Other parser problems:

- Windows CRLF files may include `\r` in `currentValue`.
- `levelHeight = y * 128` may be wrong depending on whether the file ends with a newline.
- Newlines move to the next row without processing the current token unless there was a comma before the newline.

Recommended fix:

- Process tokens on comma and newline.
- Trim `\r`.
- Process any remaining `currentValue` after the read loop ends.
- Consider using `std::getline()` and splitting each line by comma.

---

### 11. `SceneCreator::getId()` is static and never resets between level loads

File: `src/scene/scenecreator.cpp`

Current code:

```cpp
int SceneCreator::getId() {
  static int id = 2;
  id++;
  return id;
}
```

If the scene is restarted, IDs keep increasing forever. This is not a direct memory leak, but it makes entity ID assumptions unreliable and can make debugging harder.

Recommended fix:

- Make ID generation part of `SceneInfo` or a non-static scene builder instance.
- Reset the ID counter per level if IDs only need to be unique inside one level.

---

### 12. `AISystem` chooses direction and attack rectangle before direction changes

File: `src/systems/aisystem.cpp`

Current logic:

```cpp
bool facingRight = currentLevel.transformComponents.at(id)->facingRight;

if (facingRight) {
  velx = 4;
  attackRect = &attackComponent.rightCollisionRect;
} else {
  velx = -4;
  attackRect = &attackComponent.leftCollisionRect;
}

if (collisionComponent.collisionTileRight == TILE::SOLID ||
    collisionComponent.collisionTileDown == TILE::NONE) {
  velx = -4;
} else if (collisionComponent.collisionTileLeft == TILE::SOLID ||
           collisionComponent.collisionTileDown == TILE::NONE) {
  velx = 4;
}
```

Problems:

- `facingRight` is read before `velx` is changed.
- `attackRect` can represent the old direction.
- `collisionTileDown == TILE::NONE` appears in both the `if` and `else if`, so the second condition is unreachable when falling or over empty space.
- Enemy edge detection likely does not work as intended.

Recommended fix:

- Decide desired movement direction first.
- Update velocity.
- Derive facing direction from velocity.
- Select the attack rectangle after direction is finalized.

---

### 13. `Physics::attackCollision()` may fail for overlapping rectangles

File: `src/utilities/physics.cpp`

Current implementation:

```cpp
bool attackCollision(SDL_Rect const &player, SDL_Rect const &obj) {
  Velocity playerVelocity = {0, 0};
  return collisionDown(player, obj, playerVelocity) ||
         collisionUp(player, obj, playerVelocity) ||
         collisionLeft(player, obj, playerVelocity) ||
         collisionRight(player, obj, playerVelocity);
}
```

This uses directional swept-collision checks that are designed for movement. Attack hitboxes should usually use a normal AABB overlap test.

Recommended fix:

```cpp
bool attackCollision(SDL_Rect const &a, SDL_Rect const &b) {
  return checkCollision(a, b);
}
```

---

### 14. `CollisionComponent::center` initializer is incorrect

File: `src/components/collisioncomponent.hpp`

Current code:

```cpp
Utils::Position center = {(collisionRect.x + collisionRect.w + collisionRect.w) / 2,
                          (collisionRect.y + collisionRect.h + collisionRect.h) / 2};
```

This calculates approximately `(x + 2w) / 2`, not `x + w / 2`.

Recommended fix:

```cpp
Utils::Position center = {
  collisionRect.x + collisionRect.w / 2,
  collisionRect.y + collisionRect.h / 2
};
```

Or calculate the center only when needed.

---

## Performance and Inefficiency Issues

### 15. Collision detection checks every dynamic collider against every static collider

File: `src/systems/collisiondetectionsystem.cpp`

Current logic:

```cpp
for (int id : currentLevel->collisionIds) {
  ...
  for (int id : currentLevel->staticCollisionIds) {
    ...
  }
}
```

This is O(dynamic entities × static tiles) every frame.

For a tile-based platformer, collision can usually be reduced to checking nearby tiles around the entity.

Recommended fix:

- Use tile coordinates from `TransformComponent::tilePosition`.
- Check a small neighborhood around the entity.
- Or store static colliders in a spatial grid.

---

### 16. `HealthSystem` prints every entity health every frame

File: `src/systems/healthsystem.cpp`

Current code:

```cpp
std::cout << "id " << id << ", current health " << healthComponent.currentHealth
          << ", damage taken " << healthComponent.damageTaken << std::endl;
```

Printing every frame is slow and can cause visible frame drops.

Recommended fix:

- Remove this from normal gameplay.
- Guard it with a debug flag.
- Only print when damage is taken or when an entity dies.

---

### 17. `SDL_PollEvent()` is called only once per frame

File: `src/systems/controlsystem.cpp`

Current code:

```cpp
if (SDL_PollEvent(&event) != 0 && event.type == SDL_QUIT) {
  ...
}
```

This removes only one event from the queue per frame. If many events accumulate, input and window handling can lag.

Recommended fix:

```cpp
SDL_Event event;
while (SDL_PollEvent(&event)) {
  if (event.type == SDL_QUIT) {
    Scene::isRunning = false;
    System::isRunning = false;
  }
}
```

---

### 18. Position uses `int`, velocity uses `float`

Files:

- `src/components/transformcomponent.hpp`
- `src/systems/transformsystem.cpp`
- `src/utilities/utils.hpp`

`Position` is integer-based:

```cpp
struct Position {
  int x;
  int y;
};
```

`Velocity` is float-based:

```cpp
struct Velocity {
  float x;
  float y;
};
```

Movement does this:

```cpp
transformComponent.position.x = xpos + xvel;
```

Since `position.x` is an `int`, fractional velocity is truncated. This can create jitter and inconsistent acceleration.

Recommended fix:

- Store world position as `float`, and
- Convert to `int` only when creating SDL rectangles.

---

### 19. `TransformSystem::moveLeft()` can overshoot maximum speed

File: `src/systems/transformsystem.cpp`

Current code:

```cpp
if (transformComponent.velocity.x < -playerSpeedMax) {
  transformComponent.velocity.x = -playerSpeedMax;
} else {
  transformComponent.velocity.x = transformComponent.velocity.x - playerSpeed;
}
```

If velocity is `-7.9` and `playerSpeed` is `0.4`, this becomes `-8.3`.

Recommended fix:

```cpp
if (transformComponent.velocity.x - playerSpeed < -playerSpeedMax) {
  transformComponent.velocity.x = -playerSpeedMax;
} else {
  transformComponent.velocity.x -= playerSpeed;
}
```

---

### 20. `TransformSystem::update()` updates tile position using old position

File: `src/systems/transformsystem.cpp`

Current code:

```cpp
const int xpos = transformComponent.position.x;

transformComponent.position.x = xpos + xvel;

if (transformComponent.tilePosition.x != xpos / 128) {
  transformComponent.tilePosition.x = xpos / 128;
}
```

`tilePosition` is calculated from `xpos`, which is the old position.

Recommended fix:

```cpp
transformComponent.position.x = xpos + xvel;
transformComponent.tilePosition.x = transformComponent.position.x / 128;
```

The same issue exists for Y movement.

---

## Design and Architecture Notes

### 21. Some components do not inherit from `IComponent`

Files:

- `src/components/attackcomponent.hpp`
- `src/components/healthcomponent.hpp`

`AttackComponent` and `HealthComponent` are plain structs and do not derive from `IComponent`, unlike many other components.

This is not automatically wrong because the ECS stores each component type in separate maps, but it makes the component model inconsistent.

Recommended options:

1. Keep the current map-per-component style and remove `IComponent` if it is not needed.
2. Make all components consistently inherit from `IComponent`.
3. Use a proper ECS registry style where component type identity does not require a base class.

---

### 22. System pointers to `SceneInfo` are raw and nullable

Files:

- `src/scene/scene.cpp`
- `src/systems/*.hpp`

Most systems store:

```cpp
SceneInfo *currentLevel = nullptr;
```

The systems then assume `currentLevel` is valid. Calling a system before `Scene::init()` finishes would crash.

Recommended fix:

- Pass `SceneInfo&` into each `update()` call instead of storing nullable pointers, or
- Assert/check `currentLevel != nullptr` before use.

Example:

```cpp
void RenderSystem::update(SceneInfo &level);
```

---

### 23. Camera rectangle fields are used as edges, not SDL width/height

Files:

- `src/camera.cpp`
- `src/systems/rendersystem.cpp`

`Camera::viewPort` is assigned like this:

```cpp
viewPort = {offset.x, offset.y, SCREENWIDTH + offset.x, SCREENHEIGHT + offset.y};
```

For `SDL_Rect`, `w` and `h` conventionally mean width and height, not right and bottom edge.

`RenderSystem::inView()` treats them as right and bottom edges:

```cpp
destinationRect.x <= Camera::viewPort.w && destinationRect.y <= Camera::viewPort.h;
```

This works only because the code consistently treats `w` and `h` as max coordinates. It is confusing and error-prone.

Recommended fix:

- Use a custom bounds struct with `left`, `top`, `right`, and `bottom`, or
- Keep `SDL_Rect` semantics and calculate right/bottom as `x + w` and `y + h`.

---

### 24. Initialization lacks error checks

File: `src/system.cpp`

Current code:

```cpp
TTF_Init();
gFont = TTF_OpenFont("font.ttf", 32);
window = SDL_CreateWindow(...);
renderer = SDL_CreateRenderer(...);
isRunning = true;
```

Problems:

- `TTF_Init()` result is not checked.
- `TTF_OpenFont()` result is not checked.
- `SDL_CreateWindow()` result is not checked before creating a renderer.
- `SDL_CreateRenderer()` result is only checked for logging, but `isRunning` is still set to true even if renderer creation failed.

Recommended fix:

- If any critical initialization step fails, log the error and set `isRunning = false`.
- Use `TTF_GetError()` for TTF errors.
- Use `SDL_GetError()` for SDL errors.

---

### 25. `GameData::init()` can throw on missing or bad JSON

File: `src/gamedata.cpp`

Current code:

```cpp
std::ifstream f("example.json");
json data = json::parse(f);
json asd = data.at("player");
return asd;
```

Problems:

- No check that the file opened.
- `json::parse()` can throw.
- `data.at("player")` can throw.

Recommended fix:

- Check `f.is_open()`.
- Catch `nlohmann::json::exception`.
- Return an optional/error result or log failure cleanly.

---

## Positive Notes

- Most ECS component storage uses `std::unique_ptr`, which avoids many component ownership leaks automatically.
- `SDL_Surface` in `TextureManager::loadTexture()` is correctly freed after creating the texture.
- `SDL_RWops` opened in `Scene::init()` is closed in `SceneCreator::createLevel()` after successful parsing.
- Systems are separated by responsibility, which is a good foundation for further ECS refactoring.
- `RenderSystem` already avoids rendering offscreen non-static objects via `inView()`.

---

## Suggested Fix Order

1. Add `TextureManager::clean()` and destroy all `SDL_Texture*` objects.
2. Fix SDL cleanup order in `System::clean()`.
3. Add initialization error checks in `System::init()`.
4. Fix the `ControlSystem` left-movement velocity-axis bug.
5. Fix `AnimationSystem` so previous animation state is per entity/component.
6. Remove hardcoded player ID `1` from systems.
7. Fix level parser token handling.
8. Replace `Physics::attackCollision()` with normal AABB overlap.
9. Optimize collision detection to query nearby tiles only.
10. Remove or guard per-frame `std::cout` logging.

---

## Most Important Concrete Changes

### Texture cleanup

Add to `TextureManager`:

```cpp
static void clean();
```

Implementation:

```cpp
void TextureManager::clean() {
  for (SDL_Texture *texture : textures) {
    if (texture != nullptr) {
      SDL_DestroyTexture(texture);
    }
  }
  textures.clear();
}
```

Call it before SDL shutdown.

### Fix control bug

In `ControlSystem::handleInput()`:

```cpp
if (inputL && !inputR) {
  if (transformComponent.velocity.x > 0) {
    transformSystem->stopX(transformComponent);
  }
  transformSystem->moveLeft(transformComponent);
  noInput = false;
}
```

### Fix attack collision

In `Physics::attackCollision()`:

```cpp
bool attackCollision(SDL_Rect const &a, SDL_Rect const &b) {
  return checkCollision(a, b);
}
```

### Avoid per-frame health logging

Change `HealthSystem::update()` so it logs only when damage is taken or when health changes.

