/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#include "ux/draw/Rectangle.hpp"
#include "ux/sgfx/Vector.hpp"

using namespace ux::sgfx;
using namespace ux::draw;

Rectangle::Rectangle() {}

void Rectangle::draw(const DrawingScaledAttributes &attr) {
  attr.bitmap()
    .set_pen(sgfx::Pen().set_color(m_color))
    .draw_rectangle(Region(attr.point(), attr.area()));
}

RoundedRectangle::RoundedRectangle() {}

void RoundedRectangle::draw(const DrawingScaledAttributes &attr) {

  attr.bitmap().set_pen(sgfx::Pen().set_color(m_color));

  sg_size_t smallest_dimension = attr.area().width() > attr.area().height()
                                   ? attr.area().height()
                                   : attr.area().width();

  sg_size_t pixel_radius
    = (m_radius > 100 ? 100 : m_radius) * (smallest_dimension / 2) / 100;

  attr.bitmap()
    .draw_rectangle(Region( // draw center rectangle
      attr.point() + sgfx::Point(pixel_radius, 0),
      sgfx::Area(attr.area().width() - pixel_radius * 2, attr.area().height())))
    .draw_rectangle(Region( // draw left rectangle
      attr.point() + sgfx::Point(0, pixel_radius),
      sgfx::Area(pixel_radius, attr.area().height() - 2 * pixel_radius)))
    .draw_rectangle(Region( // draw right rectangle
      attr.point()
        + sgfx::Point(attr.area().width() - pixel_radius, pixel_radius),
      sgfx::Area(pixel_radius, attr.area().height() - 2 * pixel_radius)))
    .draw_quadratic_bezier( //  draw corners--top left
      attr.point() + sgfx::Point(0, pixel_radius),
      attr.point(),
      attr.point() + sgfx::Point(pixel_radius, 0))
    .draw_quadratic_bezier( // top right
      attr.point() + sgfx::Point(attr.area().width() - pixel_radius, 0),
      attr.point() + sgfx::Point(attr.area().width(), 0),
      attr.point() + sgfx::Point(attr.area().width() - 1, pixel_radius))
    .draw_quadratic_bezier( // bottom right
      attr.point()
        + sgfx::Point(
          attr.area().width() - 1,
          attr.area().height() - pixel_radius),
      attr.point()
        + sgfx::Point(attr.area().width() - 1, attr.area().height() - 1),
      attr.point()
        + sgfx::Point(
          attr.area().width() - pixel_radius,
          attr.area().height() - 1))
    .draw_quadratic_bezier( // bottom left
      attr.point() + sgfx::Point(pixel_radius, attr.area().height() - 1),
      attr.point() + sgfx::Point(0, attr.area().height() - 1),
      attr.point() + sgfx::Point(0, attr.area().height() - pixel_radius))
    .draw_pour( // now fill corners
      attr.point() + sgfx::Point(pixel_radius - 1, pixel_radius - 1),
      attr.region())
    .draw_pour(
      attr.point()
        + sgfx::Point(attr.area().width() - pixel_radius + 1, pixel_radius - 1),
      attr.region())
    .draw_pour(
      attr.point()
        + sgfx::Point(
          attr.area().width() - pixel_radius + 1,
          attr.area().height() - pixel_radius + 1),
      attr.region())
    .draw_pour(
      attr.point()
        + sgfx::Point(
          pixel_radius - 1,
          attr.area().height() - pixel_radius + 1),
      attr.region());
}
