/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
// Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

#include <cstdio>
#include <errno.h>

#include <fs/File.hpp>
#include <fs/Path.hpp>
#include <var.hpp>

#include "ux/sgfx/Font.hpp"

using namespace ux::sgfx;

Font::Info::Info(const var::StringView path) {
  m_file_path = path;

  const auto tokens = fs::Path::name(m_file_path).split("-.");

  if (tokens.count() == 3) {
    // icon font
    m_point_size = tokens.at(1).to_integer();
    m_style = Style::icons;
  } else if (tokens.count() == 4) {
    m_point_size = tokens.at(2).to_integer();
    const auto style = tokens.at(1);

    if (style == "t") {
      m_style = Style::thin;
    }
    if (style == "ti") {
      m_style = Style::thin_italic;
    }
    if (style == "el") {
      m_style = Style::extra_light;
    }
    if (style == "eli") {
      m_style = Style::extra_light_italic;
    }
    if (style == "l") {
      m_style = Style::light;
    }
    if (style == "li") {
      m_style = Style::light_italic;
    }
    if (style == "r") {
      m_style = Style::regular;
    }
    if (style == "ri") {
      m_style = Style::regular_italic;
    }
    if (style == "m") {
      m_style = Style::medium;
    }
    if (style == "mi") {
      m_style = Style::medium_italic;
    }
    if (style == "sb") {
      m_style = Style::semi_bold;
    }
    if (style == "sbi") {
      m_style = Style::semi_bold_italic;
    }
    if (style == "b") {
      m_style = Style::bold;
    }
    if (style == "bi") {
      m_style = Style::bold_italic;
    }
    if (style == "eb") {
      m_style = Style::extra_bold;
    }
    if (style == "ebi") {
      m_style = Style::extra_bold_italic;
    }
    if (style == "ico") {
      m_style = Style::icons;
    }
  } else {
    m_point_size = 0;
  }
}

var::StringView Font::Info::get_name() const {
  const auto tokens = fs::Path::name(m_file_path).split("-.");
  return tokens.at(0);
}

bool Font::Info::ascending_point_size(const Info &a, const Info &b) {
  return a.point_size() < b.point_size();
}

bool Font::Info::ascending_style(const Info &a, const Info &b) {
  return a.style() < b.style();
}

const var::StringView  Font::ascii_character_set() {
  return " !\"#$%&'()*+,-./"
         "0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
         "abcdefghijklmnopqrstuvwxyz{|}~";
}

int Font::to_charset(char ascii) {
  if ((ascii < ' ') || (ascii > '~')) {
    return -1;
  }
  return (int)(ascii - ' ' - 1);
}

Font::Font(const fs::FileObject *file) : m_file(file) {
  m_space_size = 8;
  m_letter_spacing = 1;
  m_is_kerning_enabled = true;
  refresh();
}

void Font::refresh() {

  m_file->seek(0).read(View(m_header));

  m_canvas.resize(
    Area(m_header.canvas_width, m_header.canvas_height),
    static_cast<Bitmap::BitsPerPixel>(m_header.bits_per_pixel));

  m_current_canvas = 255;
  m_canvas_start = m_header.size;
  m_canvas_size = m_canvas.view().size();

  m_kerning_pairs = var::Vector<sg_font_kerning_pair_t>();
  m_kerning_pairs.resize(m_header.kerning_pair_count);

  m_file->seek(sizeof(sg_font_header_t)).read(View(m_kerning_pairs));

  set_space_size(m_header.max_word_width);
  set_letter_spacing(m_header.max_height / 8);

  return;
}

int Font::get_width(const var::StringView  str) const {
  u32 length = 0;
  for (auto c : str) {
    if (c == ' ') {
      length += space_size();
    } else {
      if (load_char(m_char, c, true) == 0) {
        length += m_char.advance_x;
      }
    }
  }
  return length;
}

int Font::draw(char c, Bitmap &dest, const Point &point) const {

  if (load_char(m_char, c, true) < 0) {
    return -1;
  }

  Point p = point + Point(m_char.offset_x, m_char.offset_y);

  draw_char_on_bitmap(m_char, dest, p);

  return m_char.advance_x;
}

sg_font_char_t Font::character(u32 offset) {
  sg_font_char_t result = {0};
  load_char(result, offset, false);
  return result;
}

#if defined NOT_BUILDING
Bitmap Font::character_bitmap(u32 offset) {
  sg_font_char_t character = this->character(offset);

  Bitmap result(
    Area(character.width, character.height),
    Bitmap::BitsPerPixel(bits_per_pixel()));

  draw_char_on_bitmap(character, result, Point(0, 0));
  return result;
}
#endif

int Font::draw(
  const var::StringView const_string,
  Bitmap &bitmap,
  const Point &point) const {
  sg_int_t w;

  // draw characters on the bitmap
  Point p(point); // copy that can be advanced

  for (u32 i = 0; i < const_string.length(); i++) {
    char c = const_string.at(i);
    char d;
    if (i < const_string.length() - 1) {
      d = const_string.at(i + 1);
    } else {
      d = 0;
    }
    if (c == ' ') {
      w = space_size();
    } else {
      draw(c, bitmap, p);
      w = m_char.advance_x;
    }

    // apply kerning
    if (is_kerning_enabled() && (d != 0)) {
      w -= load_kerning(c, d);
    }

    p += Point(w, 0);
  }

  return 0;
}

sg_size_t Font::get_height() const { return m_header.max_height; }
sg_size_t Font::get_width() const { return m_header.max_word_width * 32; }

int Font::load_char(sg_font_char_t &ch, char c, bool ascii) const {
  int ind;
  if (ascii) {
    ind = to_charset(c);
  } else {
    ind = c;
  }

  if (ind < 0) {
    return -1;
  }

  const size_t offset
    = sizeof(sg_font_header_t)
      + sizeof(sg_font_kerning_pair_t) * m_header.kerning_pair_count
      + ind * sizeof(sg_font_char_t);

  m_file->seek(offset).read(View(ch));

  return 0;
}

int Font::load_kerning(u16 first, u16 second) const {
  int kerning_count = m_header.kerning_pair_count;
  int i;

  for (i = 0; i < kerning_count; i++) {
    if (
      (m_kerning_pairs.at(i).unicode_first == first)
      && (m_kerning_pairs[i].unicode_second == second)) {
      return m_kerning_pairs.at(i).horizontal_kerning;
    }
  }

  return 0;
}

sg_font_kerning_pair_t Font::load_kerning(u32 offset) const {
  if (offset < m_header.kerning_pair_count) {
    return m_kerning_pairs[offset];
  }
  return {0};
}

void Font::draw_char_on_bitmap(
  const sg_font_char_t &ch,
  Bitmap &dest,
  const Point &point) const {

  if (ch.canvas_idx != m_current_canvas) {
    const size_t canvas_offset = m_canvas_start + ch.canvas_idx * m_canvas_size;
    m_file->seek(canvas_offset).read(m_canvas.view());
    m_current_canvas = ch.canvas_idx;
  }

  dest.draw_sub_bitmap(
    point,
    Bitmap(m_canvas),
    Region(Point(ch.canvas_x, ch.canvas_y), Area(ch.width, ch.height)));
}
