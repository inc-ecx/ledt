#include "ScreenMain.h"

#include "Elements.h"

#include "Editor.h"

static bool isMaximized(SDL_Window* window) {
  return SDL_GetWindowFlags(window) & SDL_WINDOW_MAXIMIZED;
}

std::string ScreenMain::getMaxButtonText() {
  return isMaximized(ui->context().getWindow()) ? "rst" : "max";
}

ScreenMain::ScreenMain() {
  UiContext& context = ui->context();
  ui = Col::make({
    Cell::abs(20, Row::make({
      Cell::abs(50, Label::make("ledt")),
      Cell::rel(dragElement = Label::make("")),
      Cell::abs(40, Button::make("min", [&context] {
        SDL_MinimizeWindow(context.getWindow());
      })),
      Cell::abs(40, maxButton = Button::make(getMaxButtonText(), [&context] {
        if (isMaximized(context.getWindow()))
          SDL_RestoreWindow(context.getWindow());
        else
          SDL_MaximizeWindow(context.getWindow());
      })),
      Cell::abs(40, Button::make("ext", [] {
        SDL_Event ev;
        ev.type = SDL_EVENT_QUIT;
        SDL_PushEvent(&ev);
      })),
    })),
    Cell::rel(Border::make(2, std::make_shared<Editor>()))
  });
}

void ScreenMain::handle(const Ui::EventArgs& event) {
  if (event.se->type == SDL_EVENT_WINDOW_RESTORED || event.se->type == SDL_EVENT_WINDOW_MAXIMIZED) {
    if (maxButton) maxButton->setText(getMaxButtonText());
  }
}

bool ScreenMain::canDragWindow(int mouseX, int mouseY) {
  const auto& [x, y, width, height] = dragElement->accessBounds();
  return mouseX >= x && mouseX <= x + width
    && mouseY >= y && mouseY <= y + height;
}
