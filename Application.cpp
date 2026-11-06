#include "Application.h"

#include <stdexcept>
#include <format>

using namespace std;

Application::Application(std::unique_ptr<Screen> &&screen) :
  screen(std::move(screen)) {

}

void Application::run() {
  init();

  while (running) {
    render();
    update();
  }

  close();
}

void Application::init() {
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
    throw runtime_error("SDL_Init failed");

  if (!TTF_Init())
    throw runtime_error("TTF_Init failed");

  font = TTF_OpenFont("../LucidaConsole.ttf", 14);
  if (font == nullptr)
    throw runtime_error(std::format("TTF_OpenFont failed: {}", SDL_GetError()));

  window = SDL_CreateWindow("ledt", 640, 480, SDL_WINDOW_RESIZABLE);
  if (!window)
    throw runtime_error("SDL_CreateWindow failed");

  renderer = SDL_CreateRenderer(window, nullptr);
  if (!renderer)
    throw runtime_error("SDL_CreateRenderer failed");

  SDL_StartTextInput(window);
}

void Application::update() {
  SDL_Event e;
  while (SDL_PollEvent(&e) != 0) {
    switch (e.type) {
      case SDL_EVENT_QUIT:
        running = false;
        break;
      default:
        break;
    }

    Ui::EventArgs args{
      .se = &e
    };
    ui().handle(args);
  }
}

void Application::render() {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  Ui::RenderArgs args{
    .window = window,
    .renderer = renderer,
    .font = font
  };
  ui().render(args);

  SDL_RenderPresent(renderer);
}

void Application::close() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  TTF_Quit();
  SDL_Quit();
}

Ui & Application::ui() {
  if (screen == nullptr) throw std::runtime_error("screen is null");
  if (screen->ui == nullptr) throw std::runtime_error("screen->ui is null");
  return *screen->ui;
}
