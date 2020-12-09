// Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for
// rights.
#ifndef UXAPI_UX_LABEL_HPP
#define UXAPI_UX_LABEL_HPP

#include "Component.hpp"
#include "draw/Icon.hpp"
#include "sgfx/Font.hpp"

namespace ux {

class Label : public ComponentAccess<Label> {
public:
  Label(const var::StringView name) : ComponentAccess(name) { set_model(name); }

  void draw(const DrawingScaledAttributes &attributes);

private:
};

} // namespace ux

#endif // UXAPI_UX_LABEL_HPP
