#ifndef LEDT_SCREEN_H
#define LEDT_SCREEN_H

#include "Label.h"
#include "Ui.h"

class Screen {
public:
  virtual ~Screen() = default;

  virtual void handle(const Ui::EventArgs& event) {}

  // should return true if the point at (mouseX,mouseY) is a point the window can be dragged at.
  virtual bool canDragWindow(int mouseX, int mouseY) { return false; }

  std::shared_ptr<Ui> ui = Label::make(""); // not null. calls to the ui element are made by the application.
};

#endif //LEDT_SCREEN_H