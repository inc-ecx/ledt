#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define NOMINMAX
#include <Windows.h>

#include "Application.h"
#include "ScreenMain.h"

static Application app{};
UiContext &mainUiContext = app;

int main(int argc, char **argv) {
  SetProcessDPIAware();

  using namespace std;

  // try {
    app.run(std::make_unique<ScreenMain>());
  // } catch (std::exception const &e) {
  //   cerr << e.what() << endl;
  // }

  return 0;
}
