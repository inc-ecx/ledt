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
  Application();

  ~Application() override = default;

  void run(std::unique_ptr<Screen> &&screen);

  std::unique_ptr<Screen>& getScreen() { return theScreen; }

  // UiContext Implementation
  SDL_Window *getWindow() override { return window; };
  SDL_Renderer *getRenderer() override { return renderer; }
  int32_t mouseX() override { return mX; }
  int32_t mouseY() override { return mY; }
  void later(std::function<void()> f) override {
    if (!f) throw std::runtime_error("later(function<void()> f) called with f = nullptr");
    std::lock_guard lock{mtxLater};
    laters.push_back(f);
  }

  // SDL Handlers
  static SDL_HitTestResult HitTestCallback(SDL_Window *window, const SDL_Point *area, void *data);
  static bool EventFilter(void *data, SDL_Event *event);

private:
  void init();

  void update();

  void render();

  void close();

  void runLaters();

  void updateLayout();

  Ui &ui();
  Screen &screen();

  std::mutex mtxLater;
  std::vector<std::function<void()> > laters; // function pointers are not null


  bool running = true;

  std::mutex mtxScreen;
  TTF_Font *font = nullptr;
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  int32_t mX = std::numeric_limits<int32_t>::min();
  int32_t mY = std::numeric_limits<int32_t>::min();
  std::unique_ptr<Screen> theScreen;
};

#endif //APPLICATION_H
