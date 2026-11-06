#include "Row.h"
#include "Log.h"

static int queryWrapWidth(Cell* cell) {
  int subWidth = 0, subHeight = 0;
  bool subHasWidth = false, subHasHeight = false;
  cell->queryWrap(subWidth, subHeight, subHasWidth, subHasHeight);
  if (!subHasWidth) {
    Log::warn("Row: A wrapped cell does not provide its size");
  }
  return subWidth;
}

void Row::queryWrap(int &width, int &height, bool &hasWidth, bool &hasHeight) {
  // calculate required size to wrap this element
  hasWidth = true;
  int totalAbsolute = 0;
  for (auto &child: children) {
    const auto pCell = dynamic_cast<Cell *>(child.get());
    if (pCell->type == CellType::ABSOLUTE) {
      totalAbsolute += pCell->size;
    } else if (pCell->type == CellType::WRAP) {
      totalAbsolute += queryWrapWidth(pCell);
    }
  }
  width = totalAbsolute;

  // transfer opposite dimension
  int subWidth = 0, subHeight = 0;
  bool subHasWidth = false, subHasHeight = false;
  queryWrapChildren(subWidth, subHeight, subHasWidth, subHasHeight);
  if (subHasHeight) {
    hasHeight = true;
    height = subHeight;
  }
}

void Row::layout() {
  const dim total = bounds.width;

  // find total absolute and total relative space, also find last relative cell
  dim totalAbsolute = 0;
  dim totalRelative = 0;
  const Cell *lastRelative = nullptr;
  for (auto &child: children) {
    const auto pCell = dynamic_cast<Cell *>(child.get());
    if (pCell->type == CellType::ABSOLUTE) {
      totalAbsolute += pCell->size;
    } else if (pCell->type == CellType::WRAP) {
      totalAbsolute += queryWrapWidth(pCell);
    } else {
      totalRelative += pCell->size;
      lastRelative = pCell;
    }
  }

  // get available spacing for relative cells to divide
  dim available = total - totalAbsolute;
  if (available < 0) {
    available = 0;
  }

  // calculate bounds for each child
  dim off = 0;
  dim taken = 0;
  double remainder = 0;
  for (auto &child: children) {
    const auto pCell = dynamic_cast<Cell *>(child.get());

    // calculate size as a decimal
    double dSize = remainder;
    if (pCell->type == CellType::ABSOLUTE) {
      dSize += pCell->size;
    } else if (pCell->type == CellType::WRAP) {
      dSize += queryWrapWidth(pCell);
    } else if (totalRelative != 0) {
      dSize += todouble(pCell->size) / todouble(totalRelative) * todouble(available);
    }

    // calculate integer size
    dim size = todim(dSize);
    if (pCell == lastRelative) size = available - taken; // last relative block gets pixels left

    // update relative size which was taken
    if (pCell->type == CellType::RELATIVE) taken += size;

    // layout cell
    pCell->accessBounds()  ={bounds.x + off, bounds.y, size, bounds.height};
    pCell->accessViewport() = {viewport.x, viewport.y, viewport.width, viewport.height};

    pCell->layout();

    off += size;
    remainder = dSize - size;
  }
}