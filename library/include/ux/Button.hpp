// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

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
    const auto model_value = get_model();
    set_value(model_value.is_empty() ? name : model_value);
  }

  Button &toggle() {
    m_is_state = !m_is_state;
    return *this;
  }

  void draw(const DrawingScaledAttributes &attributes);
  void handle_event(const ux::Event &event);

  var::StringView value() const { return get_model(); }

private:
  API_ACCESS_BOOL(Button, state, false);
  chrono::ClockTimer m_hold_timer;
};

} // namespace ux

#endif // UXAPI_UX_BUTTON_HPP
