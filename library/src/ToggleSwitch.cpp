/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#include "ux/ToggleSwitch.hpp"
#include "ux/EventLoop.hpp"
#include "ux/draw/Rectangle.hpp"

using namespace ux;
using namespace ux::sgfx;

void ToggleSwitch::draw(const DrawingScaledAttributes &attributes) {

  const sg_size_t border_offset
    = attributes.height() / 100 ? attributes.height() / 100 : 1;
  const sg_size_t active_area_offset = attributes.height() / 10;
  const sg_size_t toggle_area_offset
    = active_area_offset + attributes.height() / 10;
  const sg_size_t toggle_area_total_width
    = attributes.width() - toggle_area_offset * 2;
  const sg_size_t toggle_area_active_width = toggle_area_total_width * 60 / 100;
  const sg_size_t toggle_travel
    = toggle_area_total_width - toggle_area_active_width;

  const Area toggle_area(
    toggle_area_active_width,
    attributes.height() - toggle_area_offset * 2);

  // border
  attributes.bitmap()
    .set_pen(Pen().set_color(theme()->border_color()))
    .draw_rectangle(attributes.region());

  // default
  attributes.bitmap()
    .set_pen(Pen().set_color(theme()->color()))
    .draw_rectangle(Region(
      attributes.point() + Point(border_offset, border_offset),
      attributes.area() - Area(border_offset * 2, border_offset * 2)));

  // active background
  attributes.bitmap()
    .set_pen(
      state() ? Pen().set_color(theme()->text_color())
              : Pen().set_color(theme()->border_color()))
    .draw_rectangle(Region(
      attributes.point() + Point(active_area_offset, active_area_offset),
      attributes.area()
        - Area(active_area_offset * 2, active_area_offset * 2)));

  sg_size_t toggle_point_location = 0;
  if (state() == true) {
    toggle_point_location = toggle_travel;
  }

  // default
  attributes.bitmap()
    .set_pen(Pen().set_color(theme()->color()))
    .draw_rectangle(Region(
      attributes.point()
        + Point(toggle_area_offset + toggle_point_location, toggle_area_offset),
      toggle_area));

  apply_antialias_filter(attributes);
}

void ToggleSwitch::handle_event(const ux::Event &event) {
  // change the state when an event happens in the component
  TouchContext *touch_context = event.is_trigger<TouchContext>();
  if (touch_context) {

    if (
      (event.id() == TouchContext::event_id_released)
      && contains(touch_context->point())) {
      toggle();

      if (event_loop()) {
        trigger_event(event_id_changed);
      }

      redraw();
    }
  }

  Component::handle_event(event);
}
