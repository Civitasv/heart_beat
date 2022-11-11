#pragma once

#include <cmath>
#include <iostream>

#include "point.h"
#include "render_window.h"

Point Heart(float t, Point center, float enlarge);

Point SpreadInside(Point p, Point center, float ratio);

float Curve(float p);

Point CalcPosition(Point p, Point center, float ratio);

Point Shrink(Point p, Point center, float ratio);

void RenderHeart(RenderWindow* window, int frame, int number,
                 int inside_number);
