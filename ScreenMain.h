#ifndef LEDT_SCREEN_MAIN_H
#define LEDT_SCREEN_MAIN_H

#include <complex.h>

#include "Button.h"
#include "Screen.h"

class ScreenMain : public Screen {
public:
  ScreenMain();
  ~ScreenMain() override = default;
  void handle(const Ui::EventArgs& event) override;

  std::string getMaxButtonText();
  bool canDragWindow(int mouseX, int mouseY) override;

private:
  std::shared_ptr<Ui> dragElement;
  std::shared_ptr<Button> maxButton;
};

#endif //LEDT_SCREEN_MAIN_H