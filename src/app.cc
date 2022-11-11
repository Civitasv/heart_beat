#include "app.h"

#include "heart.h"

App::App()
    : running(true),
      window("Heart Beat", 640, 480),
      frame(0),
      number(2000),
      inside_number(4000) {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    // TODO Error
    return;
  }
}

App::~App() { SDL_Quit(); }

void App::OnEvent(SDL_Event* event) {
  if (event->type == SDL_QUIT) {
    running = false;
  } else if (event->type == SDL_KEYDOWN) {
    SDL_Keycode code = event->key.keysym.sym;
    if (code == SDLK_LEFT) {
      number -= 100;
    }  // left arrow
    else if (code == SDLK_RIGHT) {
      number += 100;
    } else if (code == SDLK_DOWN) {
      inside_number -= 100;
    } else if (code == SDLK_UP) {
      inside_number += 100;
    }
  }
}

void App::OnRender() {
  window.Clear();
  RenderHeart(&window, frame, number, inside_number);
  frame = (frame + 1) % 20;
  window.Display();
}

int App::Run() {
  while (running) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      OnEvent(&event);
    }

    OnRender();
  }

  return 0;
}