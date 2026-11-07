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

  void run(std::unique_ptr<Screen>&& screen);

  std::unique_ptr<Screen>& getScreen() { return theScreen; }

  //
  // UiContext Implementation (See UiContext for threading info)
  //
  SDL_Window* getWindow() override { return window; };
  SDL_Renderer* getRenderer() override { return renderer; }
  int32_t mouseX() override { return mX; }
  int32_t mouseY() override { return mY; }
  void later(std::function<void()> f) override;

  //
  // SDL Handlers
  //
  static SDL_HitTestResult HitTestCallback(SDL_Window* window, const SDL_Point* area, void* data);
  static bool EventFilter(void* data, SDL_Event* event);

private:
  void init();

  void update();

  void close();

  //
  // must hold mtxScreen when calling these.
  //
  void render();
  void runLaters();
  void updateLayout();
  Ui& ui();
  Screen& screen();

  //
  // State
  //

  // main-thread specific
  bool running = true;

  // mtxScreen specific
  std::mutex mtxScreen;
  std::vector<std::function<void()>> laters;
  TTF_Font* font = nullptr;
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
  int32_t mX = std::numeric_limits<int32_t>::min();
  int32_t mY = std::numeric_limits<int32_t>::min();
  std::unique_ptr<Screen> theScreen;
};

#endif //APPLICATION_H
