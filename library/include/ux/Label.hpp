// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#ifndef UXAPI_UX_LABEL_HPP
#define UXAPI_UX_LABEL_HPP

#include "Component.hpp"
#include "draw/Icon.hpp"
#include "sgfx/Font.hpp"

namespace ux {

class Label : public ComponentAccess<Label> {
public:
  Label(const var::StringView name) : ComponentAccess(name) {
    const auto model_value = get_model();
    set_value(model_value.is_empty() ? name : model_value);
    set_auto_disable();
  }

  void draw(const DrawingScaledAttributes &attributes);

private:
};

} // namespace ux

#endif // UXAPI_UX_LABEL_HPP
