#ifndef LEDT_LABEL_H
#define LEDT_LABEL_H

#include <string>

#include "Ui.h"

class Label final : public Ui {
public:
  void render(const RenderArgs &a) override;

  std::string text;

  static std::shared_ptr<Label> make(const std::string& text) {
    auto instance = std::make_shared<Label>();
    instance->text = text;
    return instance;
  }
};

#endif //LEDT_LABEL_H