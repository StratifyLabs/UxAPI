// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#ifndef UXAPI_UX_DRAW_ICON_HPP_
#define UXAPI_UX_DRAW_ICON_HPP_

#include "../Drawing.hpp"
#include "../sgfx/IconFont.hpp"

namespace ux::draw {


class Icon : public Drawing, public DrawingComponentProperties<Icon> {
public:
  Icon();

  Icon &set_name(const var::StringView  value) {
    if (value.find("icon@") == 0) {
      m_name = var::String(value.get_substring_at_position(5));
    } else {
      m_name = var::String(value);
    }
    return *this;
  }

  virtual void draw(const DrawingScaledAttributes &attr);
  sgfx::Region bounds() const { return m_bounds; }

private:
  API_ACCESS_COMPOUND(Icon, var::String, icon_font_name);
  API_READ_ACCESS_COMPOUND(Icon, var::String, name);
  API_ACCESS_FUNDAMENTAL(Icon, sg_color_t, color, 0);
  sg_region_t m_bounds;
  const sgfx::IconFont *m_icon_font;

  const sgfx::IconFont *icon_font() { return m_icon_font; }

  bool resolve_font(sg_size_t h);
};

} // namespace ux::draw

#endif /* UXAPI_UX_DRAW_ICON_HPP_ */
