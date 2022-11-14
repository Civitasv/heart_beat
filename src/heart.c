#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "SDL.h"
#include "SDL_ttf.h"

#define PI 3.1415926
#define WIDTH 1280
#define HEIGHT 640

typedef SDL_FPoint Point;
typedef enum { F, T } bool;

static int font_size = 24;
static text_index = 0;

static SDL_Window* window;
static SDL_Renderer* renderer;
static bool running = T;
static int frame = 0;
static int number = 4000;
static int inside_number = 8000;
static Point* frame_pts;
static int frame_index = 0;
// color configuration
static SDL_Color fg[] = {
    {221, 83, 83}, {183, 62, 62}, {219, 200, 172}, {237, 219, 192}};
static fg_index = 0;

Point Heart(float t, Point center, float enlarge);

Point SpreadInside(Point p, Point center, float ratio);

float Curve(float p);

Point CalcPosition(Point p, Point center, float ratio);

Point Shrink(Point p, Point center, float ratio);

void RenderHeart(int frame, int number, int inside_number);

int RandomInt(int start, int end);

float RandomReal(float start, float end);

bool Has(Point* pts, Point p);

void OnEvent(SDL_Event* event);

void OnRender(char* text[], int size);

int RandomInt(int start, int end) { return start + rand() % (end - start + 1); }

float RandomReal(float start, float end) {
  float scale = rand() / (float)RAND_MAX; /* [0, 1.0] */
  return start + scale * (end - start);   /* [min, max] */
}

bool Has(Point* pts, Point p) {
  for (int i = 0; i < sizeof(pts) / sizeof(Point); i++) {
    Point item = pts[i];
    if (item.x == p.x && item.y == p.y) {
      return T;
    }
  }
  return F;
}

Point Heart(float t, Point center, float enlarge) {
  float sin_t = sin(t);
  float x = 16 * sin_t * sin_t * sin_t;
  float y = -(13 * cos(t) - 5 * cos(2 * t) - 2 * cos(3 * t) - cos(4 * t));
  x *= enlarge;
  y *= enlarge;
  x += center.x;
  y += center.y;

  Point res = {x, y};
  return res;
}

Point SpreadInside(Point p, Point center, float ratio) {
  float ratiox = -ratio * log10(RandomReal(0, 1));
  float ratioy = -ratio * log10(RandomReal(0, 1));

  float dx = ratiox * (p.x - center.x);
  float dy = ratioy * (p.y - center.y);

  Point res = {p.x - dx, p.y - dy};
  return res;
}

float Curve(float p) { return 2 * (2 * sin(4 * p)) / (2 * PI); }

Point CalcPosition(Point p, Point center, float ratio) {
  float force = 1 / pow(((p.x - center.x) * (p.x - center.x) +
                         (p.y - center.y) * (p.y - center.y)),
                        0.520);
  float dx = ratio * force * (p.x - center.x) + RandomInt(-1, 1);
  float dy = ratio * force * (p.y - center.y) + RandomInt(-1, 1);

  Point res = {p.x - dx, p.y - dy};
  return res;
}

Point Shrink(Point p, Point center, float ratio) {
  float force = -1.0f / pow(((p.x - center.x) * (p.x - center.x) +
                             (p.y - center.y) * (p.y - center.y)),
                            0.6);
  float dx = ratio * force * (p.x - center.x);
  float dy = ratio * force * (p.y - center.y);

  Point res = {p.x - dx, p.y - dy};
  return res;
}

void RenderHeart(int frame, int number, int inside_number) {
  int enlarge = 15;
  float bigger_enlarge = 16.0;
  if (number < 500) number = 500;
  if (inside_number < 500) inside_number = 500;

  float center_x = WIDTH / 2;
  float center_y = HEIGHT / 2;
  Point center = {center_x, center_y};

  Point* pts = malloc(number * sizeof(Point));
  Point* edge_pts = malloc(number * 3 * sizeof(Point));
  Point* inside_pts = malloc(inside_number * sizeof(Point));

  for (int i = 0; i < number; i++) {
    double t = RandomReal(0, 2 * PI);
    Point p = Heart(t, center, enlarge);
    pts[i] = p;
  }

  for (int i = 0; i < number; i++) {
    Point point = pts[i];
    for (int j = 0; j < 3; j++) {
      Point tp = SpreadInside(point, center, 0.05);
      edge_pts[i * 3 + j] = tp;
    }
  }
  for (int i = 0; i < inside_number; i++) {
    Point point = pts[RandomInt(0, number - 1)];
    Point tp = SpreadInside(point, center, 0.37);
    inside_pts[i] = tp;
  }

  float r = Curve(frame / 10.0f * PI);
  float ratio = 10 * r;
  int halo_radius = 3 + 6 * (1 + r);
  int halo_number = 1000 + 4000 * abs(r * r);

  Point* halo_pts = malloc(halo_number * sizeof(Point));
  int halo_index = 0;

  frame_pts = malloc((halo_number + number + number * 3 + inside_number) *
                     sizeof(Point));
  frame_index = 0;

  for (int i = 0; i < halo_number; i++) {
    float t = RandomReal(0, 2 * PI);
    Point p = Heart(t, center, bigger_enlarge);

    p = Shrink(p, center, halo_radius);
    if (Has(halo_pts, p) == F) {
      halo_pts[halo_index++] = p;
      p.x += RandomInt(-20, 20);
      p.y += RandomInt(-20, 20);
      frame_pts[frame_index++] = p;
    }
  }
  for (int i = 0; i < number; i++) {
    Point point = pts[i];
    Point p = CalcPosition(point, center, ratio);
    frame_pts[frame_index++] = p;
  }

  for (int i = 0; i < number * 3; i++) {
    Point point = edge_pts[i];
    Point p = CalcPosition(point, center, ratio);
    frame_pts[frame_index++] = p;
  }

  for (int i = 0; i < inside_number; i++) {
    Point point = inside_pts[i];
    Point p = CalcPosition(point, center, ratio);
    frame_pts[frame_index++] = p;
  }
  free(pts);
  free(edge_pts);
  free(inside_pts);
  free(halo_pts);
}

void OnEvent(SDL_Event* event) {
  if (event->type == SDL_QUIT) {
    running = F;
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

void OnRender(char* text[], int size) {
  SDL_RenderClear(renderer);

  RenderHeart(frame, number, inside_number);

  // Render Text
  TTF_Font* font = TTF_OpenFont("CascadiaCode.ttf", font_size);

  SDL_Color bg = {0, 0, 0};
  SDL_Surface* surface_message =
      TTF_RenderUTF8_LCD_Wrapped(font, text[text_index / 20], fg[3], bg, 0);
  SDL_Texture* texture =
      SDL_CreateTextureFromSurface(renderer, surface_message);
  SDL_Rect rect;
  rect.x = WIDTH / 2 - surface_message->w / 2;
  rect.y = HEIGHT / 2 - surface_message->h / 2;
  rect.w = surface_message->w;
  rect.h = surface_message->h;
  SDL_RenderCopy(renderer, texture, NULL, &rect);

  // Render Heart
  SDL_SetRenderDrawColor(renderer, fg[fg_index].r, fg[fg_index].g,
                         fg[fg_index].b, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawPointsF(renderer, frame_pts, frame_index);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderPresent(renderer);
  frame = (frame + 1) % 20;
  fg_index = (fg_index + 1) % (sizeof(fg) / sizeof(fg[0]));

  text_index = (text_index + 1) % (size * 20);

  SDL_FreeSurface(surface_message);
  SDL_DestroyTexture(texture);
}

int main(int argc, char* argv[]) {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    // TODO Error
    return;
  }
  if (TTF_Init() < 0) {
    return;
  }
  char* title = "HEART BEAT";

  if (argc > 1) {
    title = argv[1];
  }
  char** text = malloc(sizeof(char*) * 8);
  int text_size = 8;

  text[0] = "So";
  text[1] = "get away";
  text[2] = "Another way";
  text[3] = "to feel what you";
  text[4] = "didn't want yourself to know";
  text[5] = "And let yourself go";
  text[6] = "You know you didn't";
  text[7] = "lose your self-control";

  if (argc > 2) {
    text = malloc(sizeof(char*) * (argc - 2));
    text_size = argc - 2;
    for (int i = 2; i < argc; i++) {
      char* item = argv[i];
      text[i - 2] = item;
    }
  }

  window =
      SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                       WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    // ERROR!
    return;
  }
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  while (running != F) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      OnEvent(&event);
    }

    OnRender(text, text_size);
    SDL_Delay(50);
  }

  if (renderer) {
    SDL_DestroyRenderer(renderer);
  }
  if (window) {
    SDL_DestroyWindow(window);
  }

  free(frame_pts);
  TTF_Quit();
  SDL_Quit();
  return 0;
}