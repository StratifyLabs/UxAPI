/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#include "ux/Slider.hpp"
#include "ux/EventLoop.hpp"
#include "ux/TouchGesture.hpp"
#include "ux/draw/Rectangle.hpp"

using namespace ux;
using namespace ux::sgfx;

Slider::Slider(const var::String &name) : ComponentAccess(name) {
  const var::StringViewList list = Model::to_list(lookup_model_value());
  set_value(list.count() ? list.at(0).to_unsigned_long() : 0);
  set_maximum(list.count() > 1 ? list.at(1).to_unsigned_long() : 100);
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

  const sg_int_t indicator_position = m_value * indicator_range / m_maximum;

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

  if (point.x() < 50) {
    m_value = 0;
  } else if (point.x() > 950) {
    m_value = m_maximum;
  } else {
    m_value = (point.x() - 50) * m_maximum / 900;
  }

  update_model(Model::from_list<u16>({value(), maximum()}));
  redraw();
}
