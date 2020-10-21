/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#ifndef UXAPI_UX_SGFX_POINT_HPP_
#define UXAPI_UX_SGFX_POINT_HPP_

#include "Api.hpp"
#include "Area.hpp"

namespace ux::sgfx {

/*! \brief Point Class
 * \details The Point class holds a point on a Bitmap.  This
 * class is based on teh sgfx library.
 *
 */
class Point : public Api {
public:
  Point() { m_value.point = 0; }
  Point(const sg_point_t &p) : m_value(p) {}
  Point(const sg_int_t x, const sg_int_t y) {
    m_value.x = x;
    m_value.y = y;
  }

  const sg_point_t &point() const { return m_value; }
  sg_point_t &point() { return m_value; }

  operator const sg_point_t &() const { return m_value; }

  Point &set_x(sg_int_t value) {
    m_value.x = value;
    return *this;
  }

  Point &set_y(sg_int_t value) {
    m_value.y = value;
    return *this;
  }

  Point &set(sg_int_t x, sg_int_t y) {
    m_value.x = x;
    m_value.y = y;
    return *this;
  }

  sg_int_t x() const { return m_value.x; }
  sg_int_t y() const { return m_value.y; }

  void map(const sg_vector_map_t &m) { api()->point_map(&m_value, &m); }
  void unmap(const sg_vector_map_t &m) { api()->point_unmap(&m_value, &m); }

  static sg_size_t map_pixel_size(const sg_vector_map_t &m) {
    return sg_point_map_pixel_size(&m);
  }

  Point &operator=(const Point &a) {
    m_value = a;
    return *this;
  }
  bool operator==(const Point &a) const {
    return m_value.point == a.m_value.point;
  }
  bool operator!=(const Point &a) const {
    return m_value.point != a.m_value.point;
  }
  bool operator>(const Point &a) const {
    return m_value.point > a.m_value.point;
  }
  bool operator>=(const Point &a) const {
    return m_value.point >= a.m_value.point;
  }
  bool operator<=(const Point &a) const {
    return m_value.point <= a.m_value.point;
  }
  bool operator<(const Point &a) const {
    return m_value.point < a.m_value.point;
  }

  Point &operator+=(const Point &a) {
    api()->point_shift(&m_value, a);
    return *this;
  }

  Point &operator-=(const Point &a) {
    api()->point_subtract(&m_value, &a.point());
    return *this;
  }

  Point operator*(float f) const;
  Point operator+(const Point &a) const;
  Point operator-(const Point &a) const;

  void rotate(s16 angle) { api()->point_rotate(&m_value, angle); }
  void scale(u16 a) { api()->point_scale(&m_value, a); }
  void shift(s16 x, s16 y) { api()->point_shift(&m_value, sg_point(x, y)); }
  void shift(sg_point_t p) { api()->point_shift(&m_value, p); }

private:
  sg_point_t m_value;
};

} // namespace ux::sgfx

#endif /* UXAPI_UX_SGFX_POINT_HPP_ */
