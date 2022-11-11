#pragma once

#include "SDL.h"
#include "render_window.h"

class App {
 public:
  App();
  ~App();

  void OnEvent(SDL_Event* event);
  void OnRender();

  int Run();

 private:
  bool running;
  RenderWindow window;
  int frame;
  int number;
  int inside_number;
};