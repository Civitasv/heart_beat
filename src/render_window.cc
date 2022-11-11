#include "render_window.h"

RenderWindow::RenderWindow(const char* title, int w, int h) : w(w), h(h) {
  window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    // ERROR!
    return;
  }
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

RenderWindow::~RenderWindow() {
  if (renderer) {
    SDL_DestroyRenderer(renderer);
  }
  if (window) {
    SDL_DestroyWindow(window);
  }
}

void RenderWindow::Clear() { SDL_RenderClear(renderer); }

void RenderWindow::Display() { SDL_RenderPresent(renderer); }

void RenderWindow::RenderLine(int x1, int y1, int x2, int y2) {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
}

void RenderWindow::RenderPoint(int x, int y) {
  SDL_SetRenderDrawColor(renderer, 255, 113, 113, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawPoint(renderer, x, y);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
}

void RenderWindow::RenderFrame(std::vector<Point> pts) {
  SDL_SetRenderDrawColor(renderer, 255, 113, 113, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawPointsF(renderer, &pts[0], pts.size());
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
}