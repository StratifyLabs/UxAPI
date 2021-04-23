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
    m_theme(&theme) {
  m_temporary_event_stack.reserve(32);
  m_event_stack.reserve(32);
}

void EventLoop::loop() {
  m_controller.distribute_event(SystemEvent::enter());
  m_update_timer.restart();
  while (!m_is_stopped) {
    process_events();
    process_update_event();
  }
}

void EventLoop::process_update_event() {
  const MicroTime elapsed = m_update_timer.micro_time();

  if (elapsed > m_update_period) {
    m_controller.distribute_event(SystemEvent::periodic());
    m_update_timer.restart();
  } else {
    u32 remaining_milliseconds
      = m_update_period.milliseconds() - elapsed.milliseconds();
    // this should be an estimate until the next update event
    wait(remaining_milliseconds * 1_milliseconds);
  }

  m_temporary_event_stack = m_event_stack;
  m_event_stack.clear();

  while (m_temporary_event_stack.count() > 0) {
    m_controller.distribute_event(m_temporary_event_stack.back());
    m_temporary_event_stack.pop_back();
  }

}

EventLoop &EventLoop::trigger_event(const Event &event) {
  API_ASSERT(event.type() != SystemEvent::event_type());
  API_PRINTF_TRACE_LINE();
  m_event_stack.push_back(event);
  return *this;
}
