// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#ifndef UXAPI_UX_EVENTLOOP_HPP
#define UXAPI_UX_EVENTLOOP_HPP

#include <chrono/ClockTimer.hpp>
#include <var/Stack.hpp>

#include "Display.hpp"
#include "Event.hpp"
#include "sgfx/Theme.hpp"

namespace ux {

class Model;
class Controller;
class Event;

class EventLoop {
public:
  EventLoop(
    Controller &controller,
    Model &model,
    Display &display,
    const sgfx::Theme &theme);

  void loop();
  void stop(){
    m_is_stopped = true;
  }

  const chrono::ClockTimer &timer() { return m_timer; }

  /*! \details Process events should be implemented
   * to call handle_event() for each
   * event in the system that happens.
   *
   */
  virtual void process_events() = 0;

  EventLoop &trigger_event(const Event &event);

  EventLoop &set_update_period(const chrono::MicroTime &duration) {
    m_update_period = duration;
    return *this;
  }

  const sgfx::Theme *theme() const { return m_theme; }

  Display *display() { return m_display; }
  const Display *display() const { return m_display; }

  const Controller &controller() const { return m_controller; }
  Controller &controller() { return m_controller; }

  const Model &model() const { return m_model; }
  Model &model() { return m_model; }

private:
  Controller &m_controller;
  Model &m_model;
  Display *m_display = nullptr;
  const sgfx::Theme *m_theme = nullptr;
  bool m_is_stopped = false;

  chrono::ClockTimer m_timer;
  chrono::ClockTimer m_update_timer;
  chrono::MicroTime m_update_period;

  var::Vector<Event> m_event_stack;
  var::Vector<Event> m_temporary_event_stack;

  void process_update_event();
};

} // namespace ux

#endif // UXAPI_UX_EVENTLOOP_HPP
