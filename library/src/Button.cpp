// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#include "ux/Button.hpp"
#include "ux/EventLoop.hpp"
#include "ux/draw/Rectangle.hpp"
#include "ux/draw/RichText.hpp"

using namespace ux::sgfx;
using namespace ux;

void Button::draw(const DrawingScaledAttributes &attributes) {

  // draw the Border
  draw_base_properties(attributes.bitmap(), attributes.region(), theme());

  Region region_inside_padding
    = calculate_region_inside_padding(attributes.region());

  draw::RichText()
    .set_icon_font_name(theme()->primary_icon_font_name())
    .set_text_font_name(
      font_name().is_empty() ? theme()->primary_font_name() : font_name())
    .set_value(value())
    .set_color(theme()->text_color())
    .set_alignment(alignment())
    .draw(
      attributes + region_inside_padding.point()
      + region_inside_padding.area());

  apply_antialias_filter(attributes);
}

void Button::handle_event(const ux::Event &event) {
  // change the state when an event happens in the component
  TouchContext *touch_context = event.is_trigger<TouchContext>();
  if (touch_context) {

    if (theme_state() != Theme::State::disabled) {

      if (event.id() == TouchContext::event_id_dragged_point) {
        if (theme_state() == Theme::State::highlighted) {
          set_theme_state(Theme::State::default_);
          set_refresh_drawing_pending();
          m_hold_timer.stop();
        }
      }

      if (event.id() == TouchContext::event_id_released) {

        if (
          contains(touch_context->point())
          && (theme_state() == Theme::State::highlighted)) {
          toggle();
          m_hold_timer.stop();
          trigger_event(event_id_released);
        }

        if (theme_state() == Theme::State::highlighted) {
          m_hold_timer.stop();
          set_theme_state(Theme::State::default_);
          set_refresh_drawing_pending();
        }
      }

      if (
        (event.id() == TouchContext::event_id_pressed)
        && contains(touch_context->point())) {
        toggle();
        trigger_event(event_id_pressed);
        m_hold_timer.restart();

        set_theme_state(Theme::State::highlighted);
        set_refresh_drawing_pending();
      }
    }
  } else if (event.type() == SystemEvent::event_type()) {
    if (event.id() == SystemEvent::event_id_exit) {
      set_theme_state(Theme::State::default_);
      set_refresh_drawing_pending();
      m_hold_timer.reset();
    } else if (event.id() == SystemEvent::event_id_periodic) {
      if (
        m_hold_timer.is_running()
        && (m_hold_timer.micro_time() > theme()->button_hold_duration())) {
        m_hold_timer.stop();
        trigger_event(event_id_held);
      }
    }
  }
}
