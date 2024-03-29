// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#include "ux/draw/Image.hpp"

using namespace ux::draw;

Image::Image() { m_bitmap = 0; }

Image::Image(const sgfx::Bitmap &bitmap) { m_bitmap = &bitmap; }

void Image::draw(const DrawingScaledAttributes &attr) {

  sg_point_t top_left;

  if (m_bitmap != 0) {

    if (is_align_left()) {
      top_left.x = attr.point().x();
    } else if (is_align_right()) {
      top_left.x = attr.point().x() + attr.area().width() - m_bitmap->width();
    } else {
      top_left.x
        = attr.point().x() + attr.area().width() / 2 - m_bitmap->width() / 2;
    }

    if (is_align_top()) {
      top_left.y = attr.point().y();
    } else if (is_align_bottom()) {
      top_left.y = attr.point().y() + attr.area().height() - m_bitmap->height();
    } else {
      top_left.y
        = attr.point().y() + attr.area().height() / 2 - m_bitmap->height() / 2;
    }

    attr.bitmap().draw_bitmap(top_left, *m_bitmap);
  }
}
