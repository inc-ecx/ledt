#ifndef COL_H
#define COL_H

#include "Ui.h"
#include "Cell.h"

class Col final : public Ui {
public:
  void queryWrap(dim& width, dim& height, bool& hasWidth, bool& hasHeight) override;
  void layout() override;

  static std::shared_ptr<Col> make(std::vector<std::shared_ptr<Cell>>&& cells) {
    auto instance = std::make_shared<Col>();
    for (auto&& cell : cells) instance->add(std::move(cell));
    return instance;
  }
};

#endif //COL_H
