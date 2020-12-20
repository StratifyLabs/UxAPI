// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#ifndef UXAPI_UX_DRAW_TEXT_HPP_
#define UXAPI_UX_DRAW_TEXT_HPP_

#include "../Drawing.hpp"
#include "../sgfx/Font.hpp"

namespace ux::draw {

class Text : public Drawing, public DrawingComponentProperties<Text> {
public:
  virtual void draw(const DrawingScaledAttributes &attr);

  Text &set_string(const var::StringView value) {
    m_string = value;
    return *this;
  }

  Text &set_color(sg_color_t value) {
    m_color = value;
    return *this;
  }

  const sgfx::Font *font() const { return m_font; }

  Text &set_font(const sgfx::Font *font) {
    m_font = font;
    return *this;
  }

  Text &set_font_name(const var::StringView  value) {
    m_font_name = value;
    return *this;
  }

  Text &set_font_point_size(sg_size_t v) {
    m_font_point_size = v;
    return *this;
  }

  sg_size_t font_point_size() const { return m_font_point_size; }
  Text &set_font_style(sgfx::Font::Style style) {
    m_font_style = style;
    return *this;
  }

  sgfx::Font::Style font_style() const { return m_font_style; }
  sg_size_t get_width(const var::StringView sample, sg_size_t height);

protected:
  const var::StringView string() const { return m_string; }
  bool resolve_font(sg_size_t h);
  var::StringView m_string;
  var::KeyString m_font_name;
  const sgfx::Font *m_font = nullptr;
  sg_size_t m_font_point_size = 0;
  sgfx::Font::Style m_font_style = sgfx::Font::Style::regular;
  sg_color_t m_color;
};

} // namespace ux::draw

#endif /* UXAPI_UX_DRAW_TEXT_HPP_ */
