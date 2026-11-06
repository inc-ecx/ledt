#ifndef LEDT_UI_CONTEXT_H
#define LEDT_UI_CONTEXT_H

#include <cstdint>

class UiContext {
public:
  virtual ~UiContext() = default;

  virtual int32_t mouseX() = 0;
  virtual int32_t mouseY() = 0;
};

#endif //LEDT_UI_CONTEXT_H