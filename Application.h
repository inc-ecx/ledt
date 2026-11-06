#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "Ui.h"
#include "Screen.h"

class Application : public UiContext {
public:
  explicit Application(std::unique_ptr<Screen>&& screen);
  ~Application() = default;
  void run();

  int32_t mouseX() override { return mX; }
  int32_t mouseY() override { return mY; }
private:
  void init();
  void update();
  void render();
  void close();

  void updateLayout();

  Ui& ui();

  int32_t mX = std::numeric_limits<int32_t>::min();
  int32_t mY = std::numeric_limits<int32_t>::min();

  bool running = true;
  TTF_Font * font = nullptr;
  SDL_Window * window = nullptr;
  SDL_Renderer * renderer = nullptr;

  std::unique_ptr<Screen> screen;
};

#endif //APPLICATION_H
