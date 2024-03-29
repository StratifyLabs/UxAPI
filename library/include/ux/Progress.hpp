// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#ifndef UXAPI_UX_PROGRESS_HPP
#define UXAPI_UX_PROGRESS_HPP

#include "Component.hpp"
#include "Model.hpp"
#include "TouchGesture.hpp"

namespace ux {

class Progress {
public:

private:
  API_AF(Progress, u32, value, 0);
  API_AF(Progress, u32, maximum, 100);
  API_AF(Progress, u16, width, 20);
};

class ProgressBar : public ComponentAccess<ProgressBar> {
public:

  ProgressBar(const var::StringView name) : ComponentAccess(name) {
    // set_horizontal_padding(10);
    // set_vertical_padding(10);
  }

  ProgressBar &set_progress_width(u16 value) {
    progress().set_width(value);
    return *this;
  }

  using Value = var::Array<u32, 2>;
  Value value() const;

  static Value value_from_string(const var::StringView value);

  ProgressBar &set_value(u32 value, u32 maximum) {
    progress().set_value(value).set_maximum(maximum);
    set_model(
      var::NumberString(value).append("/").append(var::NumberString(maximum)));
    return *this;
  }

  ProgressBar & set_value(const Value value){
    return set_value(value.at(0), value.at(1));
  }

  void draw(const DrawingScaledAttributes &attributes);

private:
  API_AC(ProgressBar, Progress, progress);
};

} // namespace ux

#endif // UXAPI_UX_PROGRESS_HPP
