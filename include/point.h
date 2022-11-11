#pragma once

#include "SDL.h"

using Point = SDL_FPoint;

inline bool operator==(const Point& lhs, const Point& rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y;
}