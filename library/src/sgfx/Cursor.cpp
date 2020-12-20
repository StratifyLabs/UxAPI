// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#include "ux/sgfx/Cursor.hpp"
#include "printer/Printer.hpp"

printer::Printer &
printer::operator<<(printer::Printer &printer, const ux::sgfx::Cursor &a) {
  return printer;
}

using namespace ux::sgfx;

Cursor::Cursor() { m_cursor = {0}; }

Cursor::Cursor(const Bitmap &bitmap, const Point &p) {
  api()->cursor_set(&m_cursor, bitmap.bmap(), p.point());
}

Point EdgeDetector::find_next() {
  sg_size_t remaining_width = m_region.width() - m_cursor_point.x();
  sg_size_t width_until_edge;

  if (m_cursor_point.y() >= m_region.height()) {
    m_cursor_point = m_region.end_point();
    return m_region.end_point();
  }

  if (m_is_negative) {
    width_until_edge = m_cursor.find_negative_edge(remaining_width);
  } else {
    width_until_edge = m_cursor.find_positive_edge(remaining_width);
  }

  m_cursor_point += Point(width_until_edge, 0);

  Point result = m_cursor_point;
  m_is_negative = !m_is_negative;
  if (m_cursor_point.x() == m_region.width()) {
    m_is_negative = false;
    m_cursor_point.set_x(0);
    m_cursor_point += Point(0, 1);
    m_cursor.set_point(m_region.point() + m_cursor_point);
  }
  return result;
}

void EdgeDetector::draw_edges(Bitmap &bitmap, const Point &start_point) {

  Point next_edge;
  do {
    next_edge = find_next();
    if (next_edge.x() < m_region.x() + m_region.width()) {
      bitmap.draw_pixel(start_point + next_edge);
    }
  } while (next_edge < Point(m_region.width(), m_region.height()));
}
