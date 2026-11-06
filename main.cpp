#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define NOMINMAX
#include <Windows.h>

#include "Application.h"
#include "ScreenMain.h"

static Application app{std::make_unique<ScreenMain>()};
UiContext &mainUiContext = app;

int main(int argc, char **argv) {
  SetProcessDPIAware();

  using namespace std;

  try {
    app.run();
  } catch (std::exception const &e) {
    cerr << e.what() << endl;
  }

  return 0;
}
