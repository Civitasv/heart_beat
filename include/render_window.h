#pragma once

#include <vector>

#include "SDL.h"
#include "point.h"

class RenderWindow {
 public:
  RenderWindow(const char* title, int w, int h);
  ~RenderWindow();

  void Clear();

  void Display();

  void RenderLine(int x1, int y1, int x2, int y2);

  void RenderPoint(int x, int y);

  void RenderFrame(std::vector<Point> pts);

  int GetH() { return h; }
  int GetW() { return w; }

 private:
  SDL_Window* window;
  SDL_Renderer* renderer;
  int w;
  int h;
};