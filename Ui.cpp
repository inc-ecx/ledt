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

[[nodiscard]] bool Ui::hovered() const {
  //int x = Application::app.getMouseX(), y = Application::app.getMouseY();
  dim x, y;

  if (x >= (bounds.x) &&
      x < (bounds.x + bounds.width) &&
      y >= (bounds.y) &&
      y < (bounds.y + bounds.height) &&
      x >= (viewport.x) &&
      x < (viewport.x + viewport.width) &&
      y >= (viewport.y) &&
      y < (viewport.y + viewport.height))
    return true;

  return false;
}
