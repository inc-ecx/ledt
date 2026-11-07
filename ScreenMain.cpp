#include "ScreenMain.h"

#include "Elements.h"

static bool isMaximized(SDL_Window* window) {
  return SDL_GetWindowFlags(window) & SDL_WINDOW_MAXIMIZED;
}

std::string ScreenMain::getMaxButtonText() {
  return isMaximized(ui->context().getWindow()) ? "rst" : "max";
}

ScreenMain::ScreenMain() {
  UiContext& context = ui->context();
  ui = Col::make({
    Cell::abs(Row::make({
      Cell::abs(Label::make("ledt"), 50),
      Cell::rel(dragElement = Label::make("")),
      Cell::abs(Button::make("min", [&context] {
        SDL_MinimizeWindow(context.getWindow());
      }), 40),
      Cell::abs(maxButton = Button::make(getMaxButtonText(), [&context] {
        if (isMaximized(context.getWindow()))
          SDL_RestoreWindow(context.getWindow());
        else
          SDL_MaximizeWindow(context.getWindow());
      }), 40),
      Cell::abs(Button::make("ext", [] {
        SDL_Event ev;
        ev.type = SDL_EVENT_QUIT;
        SDL_PushEvent(&ev);
      }), 40),
    }), 20),
    Cell::rel(Label::make("content lol"))
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
