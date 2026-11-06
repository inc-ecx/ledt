#ifndef ROW_H
#define ROW_H

#include "Ui.h"
#include "Cell.h"

class Row final : public Ui {
public:
  void queryWrap(dim& width, dim& height, bool& hasWidth, bool& hasHeight) override;
  void layout() override;

  static std::shared_ptr<Row> make(std::vector<std::shared_ptr<Cell>>&& cells) {
    auto instance = std::make_shared<Row>();
    for (auto& cell : cells) instance->add(std::move(cell));
    return instance;
  }
};

#endif //ROW_H
