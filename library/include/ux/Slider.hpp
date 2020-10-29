/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#ifndef UXAPI_UX_SLIDER_HPP
#define UXAPI_UX_SLIDER_HPP

#include "Component.hpp"
#include "EventLoop.hpp"
#include "Model.hpp"

namespace ux {

class Slider;

class Slider : public ComponentAccess<Slider> {
public:
  enum event_ids {
    event_id_none,
    event_id_pressed,
    event_id_active,
    event_id_released,
    event_id_changed
  };

  Slider(const var::StringView name);

  void draw(const DrawingScaledAttributes &attributes);
  void handle_event(const ux::Event &event);

  var::Array<u16, 2> value() const;

  Slider &set_value(u16 value, u16 maximum) {
    set_model(
      var::NumberString(value).append("/").append(var::NumberString(maximum)));
    return *this;
  }

private:
  bool m_is_touched = false;
  void update_touch_point(const sgfx::Point display_point);

  const var::StringViewList model_list() const {
    return get_model().split("/");
  }
};

} // namespace ux

#endif // SLIDER_HPP
