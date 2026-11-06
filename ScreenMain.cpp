#include "ScreenMain.h"

#include "Elements.h"

ScreenMain::ScreenMain() {
  UiContext &context = ui->context();
  ui = Col::make({
    Cell::abs(Row::make({
      Cell::abs(Label::make("ledt"), 50),
      Cell::rel(),
      Cell::abs(Button::make("min", [&context] {
        SDL_MinimizeWindow(context.getWindow());
      }), 50),
      Cell::abs(Button::make("close", [] {
        SDL_Event ev;
        ev.type = SDL_EVENT_QUIT;
        SDL_PushEvent(&ev);
      }), 50),
    }), 20),
    Cell::rel(Label::make("content lol"))
  });
}
