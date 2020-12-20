// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#ifndef UXAPI_UX_SEPARATOR_HPP
#define UXAPI_UX_SEPARATOR_HPP

#include "Component.hpp"

namespace ux {

class Separator : public ComponentAccess<Separator> {
public:
  Separator(const var::StringView name) : ComponentAccess(name) {}
  void draw(const DrawingScaledAttributes &attributes);
};
} // namespace ux

#endif // SEPARATOR_HPP
