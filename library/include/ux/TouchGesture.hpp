// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#ifndef UXAPI_UX_TOUCHGESTURE_HPP
#define UXAPI_UX_TOUCHGESTURE_HPP

#include "Drawing.hpp"
#include "Event.hpp"
#include "sgfx/Point.hpp"
#include "sgfx/Region.hpp"
#include <chrono/ClockTimer.hpp>

namespace ux {

class TouchContext {
public:
  enum event_ids {
    event_id_none,
    event_id_active,
    event_id_pressed,
    event_id_released,
    event_id_dragged_point,
    event_id_touched,
    event_id_completed
  };

  EVENT_DECLARE_TYPE()

private:
  API_AC(TouchContext, sgfx::Point, point);
  API_AC(TouchContext, sgfx::Point, drag);
};

/*! \brief TouchGesture Class
 * \details The TouchGesture class
 * processes touch events to determine
 * if the touch was a
 *
 * - Touch (press and release)
 * - Hold (press and hold for a while)
 * - Drag (press and drag either vertically or horizontally)
 *
 */
class TouchGesture {
public:
  enum TouchContext::event_ids process(const Event &event);

  const chrono::ClockTimer &timer() const { return m_timer; }

  const sgfx::Point &drag() const { return m_drag; }

private:
  sgfx::Point m_drag;
  sgfx::Point m_last_point;
  chrono::ClockTimer m_timer;
  s16 m_vertical_drag = 0;
  s16 m_horizontal_drag = 0;
  bool m_is_vertical_drag_active = false;
  bool m_is_horizontal_drag_active = false;
  static constexpr sg_int_t m_drag_theshold = 1;

  sgfx::Point process_drag(const sgfx::Point &point);
};

} // namespace ux

#endif // UXAPI_UX_TOUCHGESTURE_HPP
