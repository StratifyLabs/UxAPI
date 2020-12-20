// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#include "ux/Slider.hpp"
#include "ux/EventLoop.hpp"
#include "ux/TouchGesture.hpp"
#include "ux/draw/Rectangle.hpp"

using namespace ux;
using namespace ux::sgfx;

Slider::Slider(const var::StringView name) : ComponentAccess(name) {}

var::Array<u16, 2> Slider::value() const {
  auto list = model_list();
  var::Array<u16, 2> result;
  result.at(0) = list.count() ? list.at(0).to_unsigned_long() : 0;
  result.at(1) = list.count() > 1 ? list.at(1).to_unsigned_long() : 100;
  return result;
}

void Slider::draw(const DrawingScaledAttributes &attributes) {

  draw_base_properties(attributes.bitmap(), attributes.region(), theme());

  const Region region_inside_margin
    = calculate_region_inside_margin(attributes.region());

  const Region slider_region
    = calculate_region_inside(region_inside_margin, {80, 5, 80, 5});

  const Area indicator_area(
    region_inside_margin.width() * 100 / 1000,
    region_inside_margin.height());

  const u16 indicator_range
    = region_inside_margin.width() - indicator_area.width();

  const var::Array<u16, 2> v = value();
  const sg_int_t indicator_position = v.at(0) * indicator_range / v.at(1);

  attributes.bitmap()
    .set_pen(sgfx::Pen().set_color(theme()->background_color()))
    .draw_rectangle(attributes.region())
    .set_pen(Pen().set_color(theme()->border_color()))
    .draw_rectangle(slider_region)
    .set_pen(Pen().set_color(theme()->color()))
    .draw_rectangle(Region(
      region_inside_margin.point() + Point(indicator_position, 0),
      indicator_area));

  apply_antialias_filter(attributes);
}

void Slider::handle_event(const ux::Event &event) {
  // change the state when an event happens in the component
  TouchContext *touch_context = event.is_trigger<TouchContext>();
  if (touch_context) {

    if (
      (event.id() == TouchContext::event_id_pressed)
      && contains(touch_context->point())) {
      m_is_touched = true;
      update_touch_point(touch_context->point());
    }

    if (m_is_touched) {
      if (event.id() == TouchContext::event_id_released) {
        m_is_touched = false;
        update_touch_point(touch_context->point());
        trigger_event(event_id_released);
      } else if (event.id() == TouchContext::event_id_active) {
        // need to check for dragging
        update_touch_point(touch_context->point());
        trigger_event(event_id_active);
      }
    }

    if (
      (event.id() == TouchContext::event_id_pressed)
      && contains(touch_context->point())) {
      m_is_touched = true;
      update_touch_point(touch_context->point());
      trigger_event(event_id_pressed);
    }
  }

  Component::handle_event(event);
}

void Slider::update_touch_point(const sgfx::Point display_point) {
  DrawingPoint point = translate_point(display_point);

  var::Array<u16, 2> v = value();

  if (point.x() < 50) {
    v.at(0) = 0;
  } else if (point.x() > 950) {
    v.at(0) = v.at(1);
  } else {
    v.at(0) = (point.x() - 50) * v.at(1) / 900;
  }

  set_value(v.at(0), v.at(1));
  redraw();
}
