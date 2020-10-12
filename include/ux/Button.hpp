/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#ifndef SAPI_UX_BUTTON_HPP
#define SAPI_UX_BUTTON_HPP

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

  Button(const var::String &name) : ComponentAccess(name) { set_value(name); }

  Button &toggle() {
    m_is_state = !m_is_state;
    return *this;
  }

  void draw(const DrawingScaledAttributes &attributes);
  void handle_event(const ux::Event &event);

private:
  API_ACCESS_COMPOUND(Button, var::String, value);
  API_ACCESS_BOOL(Button, state, false);
  chrono::Timer m_hold_timer;
};

} // namespace ux

#endif // SAPI_UX_BUTTON_HPP
