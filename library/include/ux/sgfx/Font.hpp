// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#ifndef UXAPI_UX_SGFX_FONT_HPP_
#define UXAPI_UX_SGFX_FONT_HPP_

#include <fs/File.hpp>

#include "sgfx_font_types.h"

#include "Bitmap.hpp"

namespace ux::sgfx {

class Font;

class FontFlags {
public:
  enum class IsExactMatch { no, yes };

  enum class Style {
    any,
    thin /*! Thin font (t) */,
    thin_italic /*! Thin italic (ti) */,
    extra_light /*! Extra light (el) */,
    extra_light_italic /*! Extra light italic (eli) */,
    light /*! Light (l) */,
    light_italic /*! Light italic (li) */,
    regular /*! Regular (r) */,
    regular_italic /*! Regular Italic (ri) */,
    medium /*! Medium (m) */,
    medium_italic /*! Medium italic (m) */,
    semi_bold /*! Semi bold (sb) */,
    semi_bold_italic /*! Semi bold italic (sbi) */,
    bold /*! Bold (b) */,
    bold_italic /*! Bold italic (bi) */,
    extra_bold /*! Extra bold (eb) */,
    extra_bold_italic /*! Extra bold italic (ebi) */,
    icons /*! Font is a collection of bitmap icons (ico) */,
  };
};

/*! \brief Font class
 *
 */
class Font : public Api, public FontFlags {
public:
  class Info {
  public:
    Info() {}
    Info(const var::StringView path);

    static bool ascending_point_size(const Info &a, const Info &b);
    static bool ascending_style(const Info &a, const Info &b);

    bool is_valid() const { return m_style != Style::any; }

    var::StringView get_name() const;

  private:
    API_AF(Info, Style, style, Style::any);
    API_AF(Info, sg_size_t, point_size, 0);
    API_AC(Info, var::PathString, file_path);
  };

  Font() {}
  Font(const fs::FileObject *file);

  Font(const Font &a) = delete;
  Font &operator=(const Font &a) = delete;

  Font(Font &&a) { swap(a); }
  Font &operator=(Font &&a) {
    swap(a);
    return *this;
  }


  bool is_valid() const { return m_file != nullptr; }

  static var::StringView ascii_character_set();

  sg_size_t get_height() const;
  sg_size_t get_width() const;

  int offset_x() const { return m_char.offset_x; }
  int offset_y() const { return m_char.offset_y; }
  int get_width(const var::StringView str) const;
  int size() const { return m_header.character_count; }

  Font &set_letter_spacing(sg_size_t spacing) {
    m_letter_spacing = spacing;
    return *this;
  }
  sg_size_t letter_spacing() const { return m_letter_spacing; }
  Font &set_space_size(int s) {
    m_space_size = s;
    return *this;
  }

  int space_size() const { return m_space_size; }
  const Font &draw(
    const var::StringView const_string,
    Bitmap &dest,
    const Point &point) const;

  const Font &draw(char c, Bitmap &dest, const Point &point) const;

  const sg_font_char_t &character() const { return m_char; }

  u16 character_count() const { return m_header.character_count; }
  u16 bits_per_pixel() const { return m_header.bits_per_pixel; }
  u16 kerning_pair_count() const { return m_header.kerning_pair_count; }

  Font &set_kerning_enabled(bool value = true) {
    m_is_kerning_enabled = value;
    return *this;
  }

  bool is_kerning_enabled() const { return m_is_kerning_enabled; }

  sg_font_kerning_pair_t kerning_pair(u32 offset) {
    return load_kerning(offset);
  }
  sg_font_char_t character(u32 offset);
  // Bitmap character_bitmap(u32 offset);

protected:
  const fs::FileObject *m_file = nullptr;
  mutable BitmapData m_canvas;
  mutable u8 m_current_canvas = 0;
  mutable sg_font_char_t m_char = {};
  bool m_is_kerning_enabled = true;
  sg_size_t m_letter_spacing = 1;
  int m_space_size = 8;
  sg_font_header_t m_header = {};
  u32 m_canvas_start = 0;
  u32 m_canvas_size = 0;
  var::Vector<sg_font_kerning_pair_t> m_kerning_pairs;

  void swap(Font &a) {
    std::swap(m_file, a.m_file);
    std::swap(m_canvas, a.m_canvas);
    std::swap(m_current_canvas, a.m_current_canvas);
    std::swap(m_char, a.m_char);
    std::swap(m_is_kerning_enabled, a.m_is_kerning_enabled);
    std::swap(m_letter_spacing, a.m_letter_spacing);
    std::swap(m_space_size, a.m_space_size);
    std::swap(m_header, a.m_header);
    std::swap(m_canvas_start, a.m_canvas_start);
    std::swap(m_canvas_size, a.m_canvas_size);
    std::swap(m_kerning_pairs, a.m_kerning_pairs);
  }

  void refresh();
  static int to_charset(char ascii);

  void draw_char_on_bitmap(
    const sg_font_char_t &ch,
    Bitmap &dest,
    const Point &point) const;
  int load_char(sg_font_char_t &ch, char c, bool ascii) const;
  sg_font_kerning_pair_t load_kerning(u32 offset) const;
  int load_kerning(u16 first, u16 second) const;

private:
};

} // namespace ux::sgfx

#endif /* UXAPI_UX_SGFX_FONT_HPP_ */
