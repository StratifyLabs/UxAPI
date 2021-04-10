// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#include "ux/Separator.hpp"

using namespace ux;

void Separator::draw(const DrawingScaledAttributes &attributes) {
  draw_base_properties(attributes.bitmap(), attributes.region(), theme());
}
