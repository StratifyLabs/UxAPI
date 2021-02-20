// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#ifndef UXAPI_UX_DRAW_RICHTEXT_HPP_
#define UXAPI_UX_DRAW_RICHTEXT_HPP_

#include "../Drawing.hpp"
#include "../sgfx/Font.hpp"
#include "../sgfx/IconFont.hpp"

namespace ux::draw {


class RichText : public Drawing, public DrawingComponentProperties<RichText> {
public:
  virtual void draw(const DrawingScaledAttributes &attr);

protected:
  API_ACCESS_COMPOUND(RichText, var::StringView, value);
  API_ACCESS_COMPOUND(RichText, var::StringView, text_font_name);
  API_ACCESS_COMPOUND(RichText, var::StringView, icon_font_name);
  API_ACCESS_FUNDAMENTAL(RichText, const sgfx::Font *, text_font, nullptr);
  API_ACCESS_FUNDAMENTAL(RichText, const sgfx::IconFont *, icon_font, nullptr);
  API_ACCESS_FUNDAMENTAL(RichText, sg_size_t, font_point_size, 0);
  API_ACCESS_FUNDAMENTAL(
    RichText,
    sgfx::Font::Style,
    font_style,
    sgfx::Font::Style::regular);
  API_ACCESS_FUNDAMENTAL(RichText, sg_color_t, color, 0);
  bool resolve_fonts(sg_size_t h);

  class RichToken {
  public:
    enum class Type { text, icon };

  private:
    API_ACCESS_FUNDAMENTAL(RichToken, sg_size_t, width, 0);
    API_ACCESS_FUNDAMENTAL(RichToken, sg_size_t, height, 0);
    API_ACCESS_FUNDAMENTAL(RichToken, Type, type, Type::text);
    API_ACCESS_FUNDAMENTAL(RichToken, u32, icon_index, 0);
    API_ACCESS_COMPOUND(RichToken, var::StringView, value);
  };
};

} // namespace ux::draw

#endif /* UXAPI_UX_DRAW_RICHTEXT_HPP_ */
