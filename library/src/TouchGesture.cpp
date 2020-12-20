// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#include <printer/Printer.hpp>

#include "ux/TouchGesture.hpp"

using namespace ux::sgfx;
using namespace ux;

enum TouchContext::event_ids TouchGesture::process(const Event &event) {

  TouchContext *touch_context = event.is_trigger<TouchContext>();
  if (touch_context) {
    printer::Printer p;

    if (event.id() == TouchContext::event_id_pressed) {
      // start scrolling with the touch movement
      m_is_vertical_drag_active = false;
      m_is_horizontal_drag_active = false;
      m_last_point = touch_context->point();
      m_timer.restart();
    } else {
      if (event.id() == TouchContext::event_id_active) {
        m_drag = process_drag(touch_context->point());
        if (m_is_horizontal_drag_active || m_is_vertical_drag_active) {
          return TouchContext::event_id_dragged_point;
        }
      } else if (event.id() == TouchContext::event_id_released) {
        m_timer.stop();
        if (!m_is_horizontal_drag_active && !m_is_vertical_drag_active) {
          // this was a touch
          return TouchContext::event_id_touched;
        }
      }
    }
  }

  return TouchContext::event_id_none;
}

sgfx::Point TouchGesture::process_drag(const sgfx::Point &point) {
  sgfx::Point result;
  const sgfx::Point diff = point - m_last_point;
  const sg_int_t abs_x = diff.x() > 0 ? diff.x() : -1 * diff.x();
  const sg_int_t abs_y = diff.y() > 0 ? diff.y() : -1 * diff.y();

  const sg_int_t x = abs_x > m_drag_theshold ? abs_x : 0;
  const sg_int_t y = abs_y > m_drag_theshold ? abs_y : 0;

  m_last_point = point;

  // check if both are inactive
  if (!m_is_vertical_drag_active && !m_is_horizontal_drag_active) {

    // active one drag axis at a time
    if (y > 0) {
      m_is_vertical_drag_active = true;
    }

    if (x > 0) {
      m_is_horizontal_drag_active = true;
    }
  }

  if (m_is_vertical_drag_active) {
    result.set_y(diff.y());
  }

  if (m_is_horizontal_drag_active) {
    result.set_x(diff.x());
  }

  return result;
}
