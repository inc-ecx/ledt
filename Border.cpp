#include "Border.h"

Border::~Border() {}

void Border::layout() {
  for (const auto &child: children) {
    child->accessBounds() = inset(bounds, thickness);
    child->accessViewport() = viewport;
  }
  layoutChildren();
}
