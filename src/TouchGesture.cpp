/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#include "ux/TouchGesture.hpp"

using namespace sgfx;
using namespace ux;

enum TouchContext::event_ids TouchGesture::process(const Event &event) {

  TouchContext *touch_context = event.is_trigger<TouchContext>();
  if (touch_context) {
    if (
      (event.id() == TouchContext::event_id_pressed)
      && m_region.contains(touch_context->point())) {
      // start scrolling with the touch movement
      m_is_vertical_drag_active = false;
      m_is_horizontal_drag_active = false;
      m_is_pressed_contained = true;
      m_last_point = touch_context->point();
      m_timer.restart();
    } else {
      if (m_is_pressed_contained) {

        if (event.id() == TouchContext::event_id_active) {
          m_drag = process_drag(touch_context->point());
          if (m_is_horizontal_drag_active || m_is_vertical_drag_active) {
            return TouchContext::event_id_dragged_point;
          }

        } else if (event.id() == TouchContext::event_id_released) {

          m_timer.stop();
          m_is_pressed_contained = false;
          if (!m_is_horizontal_drag_active && !m_is_vertical_drag_active) {
            // this was a touch
            return TouchContext::event_id_touched;
          }
        }
      }
    }
  }

  return TouchContext::event_id_none;
}

sgfx::Point TouchGesture::process_drag(const sgfx::Point &point) {
  sgfx::Point result;
  sgfx::Point diff = point - m_last_point;
  m_last_point = point;
  sg_int_t x = diff.x() > 0 ? diff.x() : -1 * diff.x();
  sg_int_t y = diff.y() > 0 ? diff.y() : -1 * diff.y();

  x = x > m_drag_theshold ? x : 0;
  y = y > m_drag_theshold ? y : 0;

  // check if both are inactive
  if (!m_is_vertical_drag_active && !m_is_horizontal_drag_active) {

    // should vertical scroll become active
    if (m_is_multidimensional_drag_enabled) {
      if (m_is_vertical_drag_enabled) {
        if (x > 0) {
          m_is_vertical_drag_active = true;
        }
      }

      if (m_is_horizontal_drag_enabled) {
        if (y > 0) {
          m_is_horizontal_drag_active = true;
        }
      }
    } else {
      if (m_is_vertical_drag_enabled) {
        if (m_is_horizontal_drag_enabled) {
          if (m_is_horizontal_drag_active == false) {
            // both v and h are enabled but neither are active
            // one with largest magnitude becomes active
            if (y > x) {
              if (y > 0) {
                m_is_vertical_drag_active = true;
              }
            } else {
              if (x > 0) {
                m_is_horizontal_drag_active = true;
              }
            }
          }
        } else {
          // horizontal is not enabled -- so vertical is auto-active
          if (y > 0) {
            m_is_vertical_drag_active = true;
          }
        }
      }
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
