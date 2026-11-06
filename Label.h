#ifndef LEDT_LABEL_H
#define LEDT_LABEL_H

#include <string>

#include "Ui.h"

class Label : public Ui {
public:
  void render(const RenderArgs &a) override;

  std::string text;

  static std::unique_ptr<Label> basic(const std::string& text) {
    auto instance = std::make_unique<Label>();
    instance->text = text;
    return instance;
  }
};

#endif //LEDT_LABEL_H