#include "SDL_render.h"
#include "SDL_ttf.h"
#include "SDL_video.h"

class Game {
public:
  Game() {}
  inline static SDL_Window *window = nullptr;
  inline static SDL_Renderer *renderer = nullptr;
  inline static TTF_Font *gFont = nullptr;

  inline static bool isRunning = false;

  void init(const char *title, const int xpos, const int ypos, const int width, const int height,
            const bool fullscreen);

  void clean();
  bool running();
};
