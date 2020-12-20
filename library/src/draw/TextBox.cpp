// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#include "ux/draw/TextBox.hpp"
#include "ux/Assets.hpp"
#include "var.hpp"

using namespace ux::sgfx;
using namespace ux::draw;

TextBox::TextBox() {}

int TextBox::count_lines(sg_size_t w) {
  return count_lines(this->font(), string(), w);
}

int TextBox::count_lines(
  const Font *font,
  const var::StringView string,
  sg_size_t w) {

  int num_lines;
  num_lines = 0;

  if (font == nullptr) {
    return -1;
  }

  StringViewList line_list = string.split("\n");

  for (const StringView line : line_list) {

    const StringViewList tokens = line.split(" ");
    int len;

    u32 i;
    i = 0;
    do {
      String line_to_build;
      build_line(font, i, line_to_build, tokens, len, w);
      num_lines++;
    } while (i < tokens.count());
  }

  return num_lines;
}

void TextBox::draw(const DrawingScaledAttributes &attr) {

  u32 i;
  sg_size_t w;
  sg_point_t p = attr.point();
  sg_area_t d = attr.area();
  sg_point_t start;

  sg_int_t line_y;
  sg_size_t font_height;
  sg_size_t num_lines;
  sg_size_t visible_lines;
  sg_size_t line_spacing;
  int len;

  // draw the message and wrap the text
  if (
    resolve_font(m_font_point_size == 0 ? d.height : m_font_point_size)
    == false) {
    return;
  }

  const Font *font = this->font();
  font_height = font->get_height();
  line_spacing = font_height / 10;

  // draw each line of text; make sure it doesn't overlap
  w = d.width;
  line_y = 0;

  num_lines = count_lines(w);
  if (num_lines < 0) {
    return;
  }

  visible_lines = (d.height) / (font_height + line_spacing);

  int scroll_max;
  if (num_lines <= visible_lines) {
    m_scroll = 0;
    scroll_max = 0;
  } else {
    scroll_max = num_lines - visible_lines;
  }

  if (m_scroll > scroll_max) {
    m_scroll = scroll_max;
  }

  const StringViewList line_list = string().split("\n");
  int lines_drawn = 0;
  for (const StringView input_line : line_list) {
    const StringViewList tokens = input_line.split(" ");
    String line;
    i = 0;
    do {
      line.clear();
      build_line(font, i, line, tokens, len, w);
      if (
        (lines_drawn >= m_scroll) && (lines_drawn - m_scroll < visible_lines)) {
        start.y = p.y + line_y;
        if (is_align_left()) {
          start.x = p.x;
        } else if (is_align_right()) {
          start.x = p.x + w - len;
        } else {
          start.x = p.x + (w - len) / 2;
        }
        font->draw(line, attr.bitmap(), start);
        line_y += (font_height + line_spacing);
      }
      lines_drawn++;
    } while (i < tokens.count());
  }
}

void TextBox::build_line(
  const Font *font,
  u32 &i,
  String &line,
  const StringViewList &tokens,
  int &build_len,
  sg_size_t w) {
  u32 j;
  u32 count = tokens.count();
  build_len = 0;

  for (j = i; j < count; j++) {
    const int line_len = font->get_width(line.cstring());
    const int len = font->get_width(tokens.at(i));
    if (line_len + len <= w) {
      line += tokens.at(j);
      i++;
    }

    if (len > w) {
      // single word is too large to fit on one line
      line += tokens.at(j);
      i++;
      break;
    }

    if ((line_len + len + font->space_size() <= w) && (j < (count - 1))) {
      // there is room for another word -- add a space
      line += " ";
    } else {
      break;
    }
  }

  build_len = font->get_width(line);
}
