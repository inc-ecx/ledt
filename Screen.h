#ifndef LEDT_SCREEN_H
#define LEDT_SCREEN_H

#include "Label.h"
#include "Ui.h"

class Screen {
public:
  std::unique_ptr<Ui> ui = Label::basic("");

};

#endif //LEDT_SCREEN_H