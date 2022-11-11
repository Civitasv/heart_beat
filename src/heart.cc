#include "heart.h"

#include <algorithm>
#include <random>

using namespace std;

#define PI 3.1415926

Point Heart(float t, Point center, float enlarge) {
  float sin_t = sin(t);
  float x = 16 * sin_t * sin_t * sin_t;
  float y = -(13 * cos(t) - 5 * cos(2 * t) - 2 * cos(3 * t) - cos(4 * t));
  x *= enlarge;
  y *= enlarge;
  x += center.x;
  y += center.y;

  return {x, y};
}

Point SpreadInside(Point p, Point center, float ratio) {
  random_device dev;
  mt19937 rng(dev());

  uniform_real_distribution<float> range0_1(0, 1);

  float ratiox = -ratio * log10(range0_1(rng));
  float ratioy = -ratio * log10(range0_1(rng));

  float dx = ratiox * (p.x - center.x);
  float dy = ratioy * (p.y - center.y);

  return {p.x - dx, p.y - dy};
}

float Curve(float p) { return 2 * (2 * sin(4 * p)) / (2 * PI); }

Point CalcPosition(Point p, Point center, float ratio) {
  random_device dev;
  mt19937 rng(dev());

  uniform_int_distribution<int> range_1_1(-1, 1);

  float force = 1 / pow(((p.x - center.x) * (p.x - center.x) +
                         (p.y - center.y) * (p.y - center.y)),
                        0.520);
  float dx = ratio * force * (p.x - center.x) + range_1_1(rng);
  float dy = ratio * force * (p.y - center.y) + range_1_1(rng);
  return {p.x - dx, p.y - dy};
}

Point Shrink(Point p, Point center, float ratio) {
  float force = -1.0f / pow(((p.x - center.x) * (p.x - center.x) +
                             (p.y - center.y) * (p.y - center.y)),
                            0.6);
  float dx = ratio * force * (p.x - center.x);
  float dy = ratio * force * (p.y - center.y);
  return {p.x - dx, p.y - dy};
}

void RenderHeart(RenderWindow* window, int frame, int number,
                 int inside_number) {
  int enlarge = 10;
  float bigger_enlarge = 11.0;
  if (number < 500) number = 500;
  if (inside_number < 500) inside_number = 500;

  float center_x = window->GetW() / 2;
  float center_y = window->GetH() / 2;
  std::vector<Point> pts;
  std::vector<Point> edge_pts;
  std::vector<Point> inside_pts;

  std::vector<Point> frame_pts;

  random_device dev;
  mt19937 rng(dev());

  uniform_real_distribution<float> range_0_2pi(0, 2 * PI);
  uniform_int_distribution<int> range_20_20(-20, 20);

  for (int i = 0; i < number; i++) {
    double t = range_0_2pi(rng);
    Point p = Heart(t, {center_x, center_y}, enlarge);
    pts.push_back(p);
  }
  uniform_int_distribution<int> range_0_pts_size(0, pts.size() - 1);

  for (auto& point : pts) {
    // 绘制边缘内部的点
    for (int i = 0; i < 3; i++) {
      Point tp = SpreadInside(point, {center_x, center_y}, 0.05);
      edge_pts.push_back(tp);
    }
  }
  // 再内部一点
  for (int i = 0; i < inside_number; i++) {
    Point point = pts[range_0_pts_size(rng)];
    Point tp = SpreadInside(point, {center_x, center_y}, 0.37);
    inside_pts.push_back(tp);
  }

  // 计算所有点
  float r = Curve(frame / 10.0f * PI);
  float ratio = 10 * r;
  // 光环
  int halo_radius = 3 + 6 * (1 + r);
  int halo_number = 1000 + 4000 * abs(r * r);

  std::vector<Point> halo_pts;

  for (int i = 0; i < halo_number; i++) {
    float t = range_0_2pi(rng);
    Point p = Heart(t, {center_x, center_y}, bigger_enlarge);

    p = Shrink(p, {center_x, center_y}, halo_radius);
    if (find(halo_pts.begin(), halo_pts.end(), p) == halo_pts.end()) {
      halo_pts.push_back(p);
      p.x += range_20_20(rng);
      p.y += range_20_20(rng);
      frame_pts.push_back(p);
    }
  }
  for (auto& point : pts) {
    Point p = CalcPosition(point, {center_x, center_y}, ratio);
    frame_pts.push_back(p);
  }

  for (auto& point : edge_pts) {
    Point p = CalcPosition(point, {center_x, center_y}, ratio);
    frame_pts.push_back(p);
  }

  for (auto& point : inside_pts) {
    Point p = CalcPosition(point, {center_x, center_y}, ratio);
    frame_pts.push_back(p);
  }
  window->RenderFrame(frame_pts);
}
