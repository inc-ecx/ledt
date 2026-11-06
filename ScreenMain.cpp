#include "ScreenMain.h"

#include "Elements.h"

ScreenMain::ScreenMain() {
  ui = Col::make({
    Cell::abs(Row::make({
      Cell::abs(Label::make("ledt"), 50),
      Cell::rel(),
      Cell::abs(Label::make("min"), 50),
      Cell::abs(Label::make("close"), 50),
    }), 20),
    Cell::rel(Label::make("content lol"))
  });
}
