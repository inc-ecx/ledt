#ifndef LEDT_UI_CONTEXT_H
#define LEDT_UI_CONTEXT_H

#include <cstdint>
#include <functional>

// may only be called from ui context (Ui::render, Ui::handle, Ui::layout...)
class UiContext {
public:
  virtual ~UiContext() = default;

  virtual SDL_Window *getWindow() = 0;
  virtual SDL_Renderer *getRenderer() = 0;
  virtual int32_t mouseX() = 0;
  virtual int32_t mouseY() = 0;
  virtual void later(std::function<void()> f) = 0;

};

#endif //LEDT_UI_CONTEXT_H