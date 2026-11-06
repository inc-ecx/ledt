#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define NOMINMAX
#include <Windows.h>

#include "Application.h"
#include "ScreenMain.h"

int main(int argc, char **argv) {
  SetProcessDPIAware();

  using namespace std;

  try {
    Application app{std::make_unique<ScreenMain>()};
    app.run();
  } catch (std::exception const & e) {
    cerr << e.what() << endl;
  }

  return 0;
}
