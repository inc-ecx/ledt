#ifndef BUTTON_H
#define BUTTON_H

#include <functional>
#include <string>

#include "Ui.h"

class Button : public Ui {
  std::string text;
  std::function<void()> listener;

  double hoverDuration = 0.0;

public:
  static std::shared_ptr<Button> make(const std::string& text, const std::function<void()>& listener = nullptr) {
    auto r = std::make_shared<Button>(text);
    r->listener = listener;
    return r;
  }

  explicit Button(const std::string& text) : text(text) {
  }

  void onClicked();

  // layout

  void layout() override;

  // interface

  void render(const RenderArgs& a) override;

  void handle(const EventArgs& a) override;

  // data

  void setText(const std::string& text) { this->text = text; }
};

#endif //BUTTON_H
