#include "Ui.h"

void Ui::queryWrapChildren(int& width, int& height, bool& hasWidth, bool& hasHeight) {
  for (const auto& child : children) {
    int subWidth = 0, subHeight = 0;
    bool subHasWidth = false, subHasHeight = false;
    child->queryWrap(subWidth, subHeight, subHasWidth, subHasHeight);
    if (subHasWidth) {
      width = std::max(width, subWidth);
      hasWidth = true;
    }
    if (subHasHeight) {
      height = std::max(height, subHeight);
      hasHeight = true;
    }
  }
}

void Ui::queryWrap(int& width, int& height, bool& hasWidth, bool& hasHeight) {
  queryWrapChildren(width, height, hasWidth, hasHeight);
}

void Ui::handleChildren(const EventArgs& event) {
  for (const auto& child : children) {
    child->handle(event);
  }
}

void Ui::handle(const EventArgs& event) {
  handleChildren(event);
}

void Ui::layout() {
  doLayeredLayout();
  layoutChildren();
}

void Ui::render(const RenderArgs& a) {
  renderChildren(a);
}

void Ui::doLayeredLayout() {
  for (const auto& child : children) {
    child->bounds = bounds;
    child->viewport = bounds;
  }
}

void Ui::layoutChildren() {
  for (const auto& child : children) {
    child->layout();
  }
}

void Ui::renderChildren(const RenderArgs& a) {
  for (auto itr = children.rbegin(); itr != children.rend(); ++itr) {
    (*itr)->render(a);
  }
}

//[[nodiscard]] bool Ui::hovered() const {
//  //int x = Application::app.getMouseX(), y = Application::app.getMouseY();
//
//  if (x >= (this->x) &&
//      x < (this->x + width) &&
//      y >= (this->y) &&
//      y < (this->y + height) &&
//      x >= (this->vpx1) &&
//      x < (this->vpx2) &&
//      y >= (this->vpy1) &&
//      y < (this->vpy2))
//    return true;
//
//  return false;
//}
