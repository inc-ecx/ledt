#ifndef LEDT_BORDER_H
#define LEDT_BORDER_H

#include "Ui.h"

class Border : public Ui {
public:
  ~Border() override;
  void layout() override;

  static std::shared_ptr<Border> make(dim thickness, std::shared_ptr<Ui> child) {
    auto instance = std::make_shared<Border>();
    instance->thickness = thickness;
    instance->add(std::move(child));
    return instance;
  }
private:
  dim thickness = 0;
};


#endif //LEDT_BORDER_H