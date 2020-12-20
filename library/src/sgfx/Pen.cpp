// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#include "ux/sgfx/Pen.hpp"

namespace ux::sgfx {

Pen::Pen() {
  m_pen.o_flags = 0;
  set_color((sg_color_t)-1);
  set_thickness(1);
}

} /* namespace ux::sgfx */
