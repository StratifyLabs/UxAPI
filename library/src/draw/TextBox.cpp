// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#include "ux/draw/TextBox.hpp"
#include "ux/Assets.hpp"
#include "var.hpp"

using namespace ux::sgfx;
using namespace ux::draw;

TextBox::TextBox() {}

int TextBox::count_lines(sg_size_t width) {
  return count_lines(this->font(), string(), width).line_count;
}

TextBox::CountLinesResult TextBox::count_lines(const Font *font,
  const var::StringView string,
  sg_size_t width) {

  CountLinesResult result{};
  if (font == nullptr) {
    return result;
  }

  int num_lines = 0;
  const StringViewList line_list = string.split("\n");
  for (const StringView line : line_list) {
    const StringViewList tokens = line.split(" ");
    int len;
    u32 i = 0;
    do {
      GeneralString line_to_build;
      const size_t character_count = build_line(font, i, line_to_build, tokens, len, width);
      if( result.characters_on_first_line == 0){
        result.characters_on_first_line = character_count;
      }

      //parsing empty tokens can return 0
      if( character_count ){
        result.characters_on_last_line = character_count;
      }
      num_lines++;
    } while (i < tokens.count());
  }

  result.line_count = num_lines;
  return result;
}

void TextBox::draw(const DrawingScaledAttributes &attr) {

  u32 i;
  const sg_point_t p = attr.point();
  const sg_area_t d = attr.area();

  int len;

  // draw the message and wrap the text
  if (
    resolve_font(m_font_point_size == 0 ? d.height : m_font_point_size)
    == false) {
    return;
  }

  const Font *font = this->font();
  const sg_size_t font_height = font->get_height();
  const sg_size_t line_spacing = font_height / 10;

  // draw each line of text; make sure it doesn't overlap
 const sg_size_t width = d.width;

  const sg_size_t num_lines = count_lines(width);
  if (num_lines < 0) {
    return;
  }

  const sg_size_t visible_lines = (d.height) / (font_height + line_spacing);

  if (num_lines <= visible_lines) {
    m_scroll = 0;
    m_scroll_total = 0;
  } else {
    m_scroll_total = num_lines - visible_lines;
  }

  if (m_scroll > m_scroll_total) {
    m_scroll = m_scroll_total;
  }

  const StringViewList line_list = string().split("\n");
  sg_int_t line_y = 0;
  int lines_drawn = 0;
  sg_point_t start{};
  const auto y_spacing = font_height + line_spacing;
  bool is_first_line_assigend = false;
  for (const StringView input_line : line_list) {
    const StringViewList tokens = input_line.split(" ");
    i = 0;
    do {
      GeneralString line;
      const auto character_length = build_line(font, i, line, tokens, len, width);
      if( is_first_line_assigend == false ){
        is_first_line_assigend = true;
        m_first_line_length = character_length;
      }
      if (
        (lines_drawn >= m_scroll) && (lines_drawn - m_scroll < visible_lines)) {
        start.y = p.y + line_y;
        if (is_align_left()) {
          start.x = p.x;
        } else if (is_align_right()) {
          start.x = p.x + width - len;
        } else {
          start.x = p.x + (width - len) / 2;
        }
        font->draw(line, attr.bitmap(), start);
        line_y += (y_spacing);
      }
      lines_drawn++;
    } while (i < tokens.count());
  }
}

size_t TextBox::build_line(const Font *font,
  u32 &token_position,
  GeneralString &line,
  const StringViewList &tokens,
  int &build_len,
  sg_size_t width) {
  build_len = 0;
  size_t result = 0;

  const u32 count = tokens.count();
  for (u32 j = token_position; j < count; j++) {
    const int line_len = font->get_width(line);
    const int len = font->get_width(tokens.at(token_position));
    if (line_len + len <= width) {
      const auto token = tokens.at(j);
      line |= token;
      result += token.length();
      token_position++;
    }

    if (len > width) {
      // single word is too large to fit on one line
      const auto token = tokens.at(j);
      result += token.length();
      line |= token;
      token_position++;
      break;
    }

    if ((line_len + len + font->space_size() <= width) && (j < (count - 1))) {
      // there is room for another word -- add a space
      line |= " ";
      result++;
    } else {
      break;
    }
  }

  build_len = font->get_width(line);

  //returns the number of characters used on the line
  return result;
}
