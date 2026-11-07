#include "Application.h"

#include <stdexcept>
#include <format>
#include <cassert>

#include "Log.h"

using namespace std;

Application::Application() {}

void Application::run(std::unique_ptr<Screen>&& screen) {
  init();

  {
    std::lock_guard lock{mtxScreen};
    this->theScreen.swap(screen);
    updateLayout();
  }

  while (running) {
    update();

    {
      std::lock_guard lock{mtxScreen};
      render();
      runLaters();
    }
  }

  close();
}

SDL_HitTestResult Application::HitTestCallback(SDL_Window* window, const SDL_Point* area, void* data) {
  constexpr int MOUSE_RESIZE_PADDING = 8;

  assert(data);
  auto app = static_cast<Application*>(data);

  {
    std::lock_guard lock{app->mtxScreen};
    std::unique_ptr<Screen>& screen = app->getScreen();
    if (screen != nullptr && screen->canDragWindow(area->x, area->y))
      return SDL_HITTEST_DRAGGABLE;
  }

  int width, height;
  SDL_GetWindowSize(window, &width, &height);

  if ((SDL_GetWindowFlags(window) & SDL_WINDOW_MAXIMIZED) == 0) {
    if (area->y < MOUSE_RESIZE_PADDING) {
      //if (area->x < MOUSE_RESIZE_PADDING) {
      //  return SDL_HITTEST_RESIZE_TOPLEFT;
      //} else if (area->x > width - MOUSE_RESIZE_PADDING) {
      //  return SDL_HITTEST_RESIZE_TOPRIGHT;
      //} else {
      //  return SDL_HITTEST_RESIZE_TOP;
      //}
    }
    else if (area->y > height - MOUSE_RESIZE_PADDING) {
      if (area->x < MOUSE_RESIZE_PADDING) {
        return SDL_HITTEST_RESIZE_BOTTOMLEFT;
      }
      else if (area->x > width - MOUSE_RESIZE_PADDING) {
        return SDL_HITTEST_RESIZE_BOTTOMRIGHT;
      }
      else {
        return SDL_HITTEST_RESIZE_BOTTOM;
      }
    }
    else if (area->x < MOUSE_RESIZE_PADDING) {
      return SDL_HITTEST_RESIZE_LEFT;
    }
    else if (area->x > width - MOUSE_RESIZE_PADDING) {
      return SDL_HITTEST_RESIZE_RIGHT;
    }
  }

  return SDL_HITTEST_NORMAL;
}

bool Application::EventFilter(void* data, SDL_Event* event) {
  if (!data || !event) return false;

  auto app = static_cast<Application*>(data);

  if (event->type == SDL_EVENT_WINDOW_EXPOSED) {
    app->updateLayout();

    {
      std::lock_guard lock{app->mtxScreen};
      app->mX = std::numeric_limits<int>::min();
      app->mY = std::numeric_limits<int>::min();

      Ui::EventArgs args{
        .se = event
      };
      app->screen().handle(args);
      app->ui().handle(args);
    }
    app->render();
  }

  return true;
}

void Application::init() {
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
    throw runtime_error("SDL_Init failed");

  if (!TTF_Init())
    throw runtime_error("TTF_Init failed");

  font = TTF_OpenFont("../LucidaConsole.ttf", 14);
  if (font == nullptr)
    throw runtime_error(std::format("TTF_OpenFont failed: {}", SDL_GetError()));

  window = SDL_CreateWindow("ledt", 640, 480, SDL_WINDOW_RESIZABLE | SDL_WINDOW_BORDERLESS);
  if (!window)
    throw runtime_error("SDL_CreateWindow failed");

  if (!SDL_SetWindowHitTest(window, HitTestCallback, this))
    throw runtime_error("SDL_SetWindowHitTest failed");

  if (!SDL_AddEventWatch(EventFilter, this))
    throw runtime_error("SDL_AddEventWatch failed");

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
    case SDL_EVENT_WINDOW_RESIZED:
      updateLayout();
      break;
    case SDL_EVENT_MOUSE_MOTION: {
      std::lock_guard lock{mtxScreen};
      mX = static_cast<int>(e.motion.x);
      mY = static_cast<int>(e.motion.y);
      break;
    }
    default:
      break;
    }

    {
      std::lock_guard lock{mtxScreen};
      Ui::EventArgs args{
        .se = &e
      };
      screen().handle(args);
      ui().handle(args);
    }
  }
}

void Application::render() {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  int w, h;
  SDL_GetWindowSize(window, &w, &h);
  SDL_FRect rect = {
    .x = 0.0f,
    .y = 0.0f,
    .w = static_cast<float>(w),
    .h = static_cast<float>(h)
  };
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderRect(renderer, &rect);

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

void Application::runLaters() {
  for (auto f : laters) f();
  laters.clear();
}

void Application::updateLayout() {
  int w, h;
  SDL_GetWindowSize(window, &w, &h);
  if (w < 1) w = 1;
  if (h < 1) h = 1;

  if (ui().accessBounds().width == w && ui().accessBounds().height == h) return;

  ui().accessBounds() = {0, 0, Ui::todim(w), Ui::todim(h)};
  ui().layout();
}

Screen& Application::screen() {
  if (theScreen == nullptr) throw std::runtime_error("screen is null");
  return *theScreen;
}

Ui& Application::ui() {
  if (theScreen == nullptr) throw std::runtime_error("screen is null");
  if (theScreen->ui == nullptr) throw std::runtime_error("screen->ui is null");
  return *theScreen->ui;
}

void Application::later(std::function<void()> f) {
  if (!f) throw std::runtime_error("later(function<void()> f) called with f = nullptr");
  laters.push_back(f);
}
