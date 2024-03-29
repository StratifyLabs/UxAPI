// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#ifndef UXAPI_UX_SGFX_AREA_HPP_
#define UXAPI_UX_SGFX_AREA_HPP_

#include "sgfx_types.h"

namespace ux::sgfx {

/*! \brief Areaension Class
 * \details This class holds a dimension for bitmaps.
 * It is based on the sgfx library.
 */
class Area {
public:
  Area();
  Area(sg_size_t width, sg_size_t height) {
    m_value.width = width;
    m_value.height = height;
  }

  Area(const sg_area_t &d) : m_value(d) {}

  bool is_valid() const { return (m_value.width && m_value.height); }

  Area &set_width(sg_size_t w) {
    m_value.width = w;
    return *this;
  }
  Area &set_height(sg_size_t h) {
    m_value.height = h;
    return *this;
  }

  bool operator==(const Area &a) const {
    return m_value.area == a.m_value.area;
  }

  bool operator!=(const Area &a) const {
    return m_value.area != a.m_value.area;
  }

  bool operator>(const Area &a) const {
    return m_value.width * m_value.height > a.m_value.width * a.m_value.height;
  }

  bool operator<(const Area &a) const {
    return m_value.width * m_value.height < a.m_value.width * a.m_value.height;
  }

  bool operator>=(const Area &a) const {
    return m_value.width * m_value.height >= a.m_value.width * a.m_value.height;
  }

  bool operator<=(const Area &a) const {
    return m_value.width * m_value.height <= a.m_value.width * a.m_value.height;
  }

  Area operator*(float a) const {
    Area d;
    d.m_value.width = m_value.width * a;
    d.m_value.height = m_value.height * a;
    return d;
  }

  Area operator*(sg_size_t a) const {
    Area d;
    d.m_value.width = m_value.width * a;
    d.m_value.height = m_value.height * a;
    return d;
  }

  Area operator/(sg_size_t a) const {
    Area d;
    d.m_value.width = m_value.width / a;
    d.m_value.height = m_value.height / a;
    return d;
  }

  Area &operator*=(float a) {
    m_value.width *= a;
    m_value.height *= a;
    return *this;
  }

  Area operator+(const Area &a) {
    return Area(width() + a.width(), height() + a.height());
  }

  Area operator-(const Area &a) {
    return Area(width() - a.width(), height() - a.height());
  }

  u32 calculate_area() const { return m_value.width * m_value.height; }

  sg_size_t width() const { return m_value.width; }
  sg_size_t height() const { return m_value.height; }

  sg_size_t maximum_dimension() const {
    return m_value.width > m_value.height ? m_value.width : m_value.height;
  }

  sg_size_t minimum_dimension() const {
    return m_value.width < m_value.height ? m_value.width : m_value.height;
  }

  const sg_area_t &area() const { return m_value; }
  sg_area_t &area() { return m_value; }

  operator const sg_area_t &() const { return m_value; }

private:
  sg_area_t m_value;
};

} // namespace ux::sgfx

#endif /* UXAPI_UX_SGFX_AREA_HPP_ */
