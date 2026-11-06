#ifndef LEDT_SCREEN_H
#define LEDT_SCREEN_H

#include "Label.h"
#include "Ui.h"

class Screen {
public:
  std::shared_ptr<Ui> ui = Label::make("");
};

#endif //LEDT_SCREEN_H