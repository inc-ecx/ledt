#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "Ui.h"
#include "Screen.h"

class Application {
public:
  explicit Application(std::unique_ptr<Screen>&& screen);
  void run();
private:
  void init();
  void update();
  void render();
  void close();

  Ui& ui();

  bool running = true;
  TTF_Font * font = nullptr;
  SDL_Window * window = nullptr;
  SDL_Renderer * renderer = nullptr;

  std::unique_ptr<Screen> screen;
};

#endif //APPLICATION_H
