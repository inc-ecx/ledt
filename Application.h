#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>
#include <mutex>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "Ui.h"
#include "Screen.h"

class Application : public UiContext {
public:
  explicit Application(std::unique_ptr<Screen> &&screen);

  ~Application() override = default;

  void run();

  // UiContext Implementation
  SDL_Window *getWindow() override { return window; };
  SDL_Renderer *getRenderer() override { return renderer; }
  int32_t mouseX() override { return mX; }
  int32_t mouseY() override { return mY; }
  void later(std::function<void()> f) override {
    if (!f) throw std::runtime_error("later(function<void()> f) called with f = nullptr");
    std::lock_guard lock(mtxLater);
    laters.push_back(f);
  }

private:
  void init();

  void update();

  void render();

  void close();

  void runLaters();

  void updateLayout();

  Ui &ui();

  std::mutex mtxLater;
  std::vector<std::function<void()> > laters; // function pointers are not null

  int32_t mX = std::numeric_limits<int32_t>::min();
  int32_t mY = std::numeric_limits<int32_t>::min();

  bool running = true;
  TTF_Font *font = nullptr;
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;

  std::unique_ptr<Screen> screen;
};

#endif //APPLICATION_H
