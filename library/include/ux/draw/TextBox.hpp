// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#ifndef UXAPI_UX_DRAW_TEXTBOX_HPP_
#define UXAPI_UX_DRAW_TEXTBOX_HPP_

#include "Text.hpp"

namespace ux::draw {

class TextBox : public Text {
public:
  TextBox();

  struct CountLinesResult {
    size_t line_count;
    size_t characters_on_first_line;
    size_t characters_on_last_line;
  };

  static CountLinesResult count_lines(
    const sgfx::Font *font,
    const var::StringView string,
    sg_size_t width);

  virtual void draw(const DrawingScaledAttributes &attr);

  static constexpr sg_size_t maximum_scroll() { return 65535; }

  TextBox &decrement_scroll() {
    if (m_scroll) {
      m_scroll--;
    }
    return *this;
  }

  TextBox &increment_scroll() {
    if (m_scroll < 65535) {
      m_scroll++;
    }
    return *this;
  }

  size_t build_line(u32 &token_position,
    var::GeneralString &line,
    const var::StringViewList &tokens, sg_size_t width) const;

private:
  API_AF(TextBox, sg_size_t, scroll, 0);
  API_RAF(TextBox, sg_size_t, scroll_total, 0);
  API_RAF(TextBox, sg_size_t, first_line_length, 0);

  int count_lines(sg_size_t width);

  static size_t build_line(
    const sgfx::Font *font,
    u32 &token_position,
    var::GeneralString &line,
    const var::StringViewList &tokens,
    int &build_len,
    sg_size_t width);
};

} // namespace ux::draw

#endif /* UXAPI_UX_DRAW_TEXTBOX_HPP_ */
