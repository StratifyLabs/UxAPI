// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#include <cmath>

#include "ux/sgfx/Point.hpp"
using namespace ux::sgfx;

Point Point::operator*(float f) const {
  float x, y;
  x = f * this->x();
  y = f * this->y();
  return Point(rintf(x), rintf(y));
}

Point Point::operator+(const Point &a) const {
  Point p(*this);
  api()->point_shift(&p.point(), a);
  return p;
}

Point Point::operator-(const Point &a) const {
  Point p(*this);
  api()->point_subtract(&p.point(), &a.point());
  return p;
}
