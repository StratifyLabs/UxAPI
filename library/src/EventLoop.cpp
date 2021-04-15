// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#include "ux/EventLoop.hpp"
#include "chrono.hpp"
#include "ux/Controller.hpp"
#include "ux/Model.hpp"

using namespace ux;

EventLoop::EventLoop(
  Controller &controller,
  Model &model,
  Display &display,
  const sgfx::Theme &theme)
  : m_controller(controller), m_model(model), m_display(&display),
    m_theme(&theme) {}

void EventLoop::loop() {
  m_controller.distribute_event(SystemEvent(SystemEvent::event_id_enter));
  m_update_timer.restart();
  while (!m_is_stopped) {
    process_events();
    process_update_event();
  }
}

void EventLoop::process_update_event() {
  const MicroTime elapsed = m_update_timer.micro_time();

  if (elapsed > m_update_period) {
    m_controller.distribute_event(SystemEvent(SystemEvent::event_id_periodic));
    m_update_timer.restart();
  } else {
    u32 remaining_milliseconds
      = m_update_period.milliseconds() - elapsed.milliseconds();
    // this should be an estimate until the next update event
    wait(MicroTime(remaining_milliseconds * 1000));
  }

  std::swap(m_temporary_event_stack, m_event_stack);

  while (m_temporary_event_stack.count() > 0) {
    m_controller.distribute_event(m_temporary_event_stack.top());
    m_temporary_event_stack.pop();
  }
}

EventLoop &EventLoop::trigger_event(const Event &event) {
  API_ASSERT(event.type() != SystemEvent::event_type());
  m_event_stack.push(event);
  return *this;
}
