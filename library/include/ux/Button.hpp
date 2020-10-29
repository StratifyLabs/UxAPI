/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#ifndef UXAPI_UX_BUTTON_HPP
#define UXAPI_UX_BUTTON_HPP

#include "Component.hpp"
#include "TouchGesture.hpp"
#include "draw/Icon.hpp"

namespace ux {

class Button : public ComponentAccess<Button> {
public:
  enum event_ids {
    event_id_none,
    event_id_active,
    event_id_pressed,
    event_id_released,
    event_id_held
  };

  Button(const var::StringView name) : ComponentAccess(name) {
    set_value(name);
  }

  Button &toggle() {
    m_is_state = !m_is_state;
    return *this;
  }

  void draw(const DrawingScaledAttributes &attributes);
  void handle_event(const ux::Event &event);

private:
  API_ACCESS_COMPOUND(Button, var::KeyString, value);
  API_ACCESS_BOOL(Button, state, false);
  chrono::ClockTimer m_hold_timer;
};

} // namespace ux

#endif // UXAPI_UX_BUTTON_HPP
