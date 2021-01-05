// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#ifndef UXAPI_UX_CONTROLLER_HPP
#define UXAPI_UX_CONTROLLER_HPP

#include "Event.hpp"
#include "Layout.hpp"
#include "Model.hpp"

namespace ux {

class EventLoop;

class Controller : public api::ExecutionContext {
public:
  explicit Controller(EventLoop &event_loop) : m_event_loop(event_loop) {}

  void transition(Layout &next_layout);
  void distribute_event(const ux::Event &event);
  virtual void handle_event(const ux::Event &event) {}

  Model &model();
  const Model &model() const;

  Layout *current_layout() { return m_current_layout; }
  const Layout *current_layout() const { return m_current_layout; }

private:
  Layout *m_current_layout = nullptr;
  Layout *m_next_layout = nullptr;
  EventLoop &m_event_loop;

  void refresh_drawing(Layout *m_current_layout);
};

} // namespace ux
#endif // UXAPI_UX_CONTROLLER_HPP
