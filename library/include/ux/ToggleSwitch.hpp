// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#ifndef UXAPI_UX_TOGGLESWITCH_HPP
#define UXAPI_UX_TOGGLESWITCH_HPP

#include "Component.hpp"
#include "Event.hpp"
#include "TouchGesture.hpp"

namespace ux {

class ToggleSwitch;

class ToggleSwitch : public ComponentAccess<ToggleSwitch> {
public:
  enum event_ids { event_id_none, event_id_changed };

  ToggleSwitch(const var::StringView name) : ComponentAccess(name) {}

  bool state() const { return m_state; }

  ToggleSwitch &set_state(bool value) {
    m_state = value;
    set_model(value ? "true" : "false");
    return *this;
  }

  ToggleSwitch &toggle() {
    m_state = !m_state;
    set_model(m_state ? "true" : "false");
    return *this;
  }

  void draw(const DrawingScaledAttributes &attributes);
  void handle_event(const ux::Event &event);

private:
  bool m_state;
};

} // namespace ux

#endif // UXAPI_UX_TOGGLESWITCH_HPP
