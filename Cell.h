#ifndef CELL_H
#define CELL_H

#include "Ui.h"

enum class CellType {
  ABSOLUTE, RELATIVE, WRAP
};

// A cell is the child of a column or a row.

class Cell final : public Ui {
public:
  CellType type;
  dim size;

  Cell(CellType const type, dim const size, std::shared_ptr<Ui> child) :
    Ui(),
    type(type),
    size(size) {
    add(std::move(child));
  }

  Cell(CellType const type, dim const size) :
    type(type),
    size(size) {}

  ~Cell() override = default;

  static std::shared_ptr<Cell> abs(std::shared_ptr<Ui> child, dim size) {
    return std::make_shared<Cell>(CellType::ABSOLUTE, size, std::move(child));
  }

  static std::shared_ptr<Cell> rel(std::shared_ptr<Ui> child, dim size = 1) {
    return std::make_shared<Cell>(CellType::RELATIVE, size, std::move(child));
  }

  static std::shared_ptr<Cell> abs(dim size) {
    return std::make_shared<Cell>(CellType::ABSOLUTE, size);
  }

  static std::shared_ptr<Cell> rel(dim size = 1) {
    return std::make_shared<Cell>(CellType::RELATIVE, size);
  }

  // this uses the biggest provided absolute size by any child in the hierarchy. (provided using Ui::queryWrap)
  // this allows for nested rows or cols to effectively have their own absolute size, without hardcoding it.
  // example: Row::make(
  //   ... some cells
  //   Cell::wrap(
  //     ... some hierarchy
  //       Row::make({
  //         Cell::abs(Button::make("Hi1"), 100),
  //         Cell::abs(Button::make("Hi2"), 100)
  //       }),
  //     ... end of some hierarchy
  //   ), // equivalent of Cell::abs(..., 200)
  //   ...
  // )
  static std::shared_ptr<Cell> wrap(std::shared_ptr<Ui> child) {
    return std::make_shared<Cell>(CellType::WRAP, 0, std::move(child));
  }
};

#endif //CELL_H
