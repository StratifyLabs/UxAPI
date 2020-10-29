/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc

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
    Info();

    /*! \details Enables sorting Info objects by point size. */
    static bool ascending_point_size(const Info &a, const Info &b);
    /*! \details Enables sorting Info objects by style. */
    static bool ascending_style(const Info &a, const Info &b);

  private:
    API_AF(Info, Style, style, Style::any);
    API_AF(Info, sg_size_t, point_size, 0);
    API_AC(Info, var::PathString, file_path);
  };

  Font(const fs::File &file);

  /*! \details Returns a string of the available character set */
  static const var::StringView  ascii_character_set();

  /*! \details Returns the maximum height of any character in the font. */
  sg_size_t get_height() const;

  /*! \details Returns the maximum width of any character in the font. */
  sg_size_t get_width() const;

  // Attribute access methods
  int offset_x() const { return m_char.offset_x; }
  int offset_y() const { return m_char.offset_y; }

  /*! \details Calculates the length (pixels on x-axis) of the specified string.
   */
  int get_width(const var::StringView  str) const;

  /*! \details Returns the number of characters in the font. */
  int size() const { return m_header.character_count; }

  /*! \details Sets the spacing between letters within a word. */
  void set_letter_spacing(sg_size_t spacing) { m_letter_spacing = spacing; }

  /*! \details Returns the spacing of the letters within a word. */
  sg_size_t letter_spacing() const { return m_letter_spacing; }

  /*! \details Sets the number of pixels in a space between words. */
  void set_space_size(int s) { m_space_size = s; }

  /*! \details Returns the number of pixels between words. */
  int space_size() const { return m_space_size; }

  /*! \details Draws the string on the specified bitmap.
   *
   * @param str The string to draw (or set)
   * @param dest The bitmap to draw the string on
   * @param point The top left corner to start drawing the string
   * @return Zero on success
   */
  int draw(const var::StringView const_string, Bitmap &dest, const Point &point)
    const;

  /*! \details Draws a character on the bitmap.
   *
   * @param c The character to draw
   * @param dest The destination bitmap
   * @param point The top-left corner where to draw on \a dest
   * @return Zero on success (-1 if character fails to load)
   */
  int draw(char c, Bitmap &dest, const Point &point) const;

  const sg_font_char_t &character() const { return m_char; }

  /*! \details Accesses the number of characters in the font. */
  u16 character_count() const { return m_header.character_count; }
  /*! \details Accesses the number of bits per pixel in the font. */
  u16 bits_per_pixel() const { return m_header.bits_per_pixel; }
  /*! \details Accesses the number of kerning pairs in the font. */
  u16 kerning_pair_count() const { return m_header.kerning_pair_count; }

  /*! \details Enables (or disables) kerning. */
  Font &set_kerning_enabled(bool value = true) {
    m_is_kerning_enabled = value;
    return *this;
  }

  /*! \details Returns true if kerning is enabled. */
  bool is_kerning_enabled() const { return m_is_kerning_enabled; }

  sg_font_kerning_pair_t kerning_pair(u32 offset) {
    return load_kerning(offset);
  }
  sg_font_char_t character(u32 offset);
  Bitmap character_bitmap(u32 offset);

protected:
  /*! \cond */
  mutable sg_font_char_t m_char = {0};
  bool m_is_kerning_enabled = true;
  sg_size_t m_letter_spacing = 1;
  int m_space_size = 8;
  sg_font_header_t m_header = {0};
  mutable BitmapData m_canvas;
  mutable u8 m_current_canvas = 0;
  u32 m_canvas_start = 0;
  u32 m_canvas_size = 0;
  var::Vector<sg_font_kerning_pair_t> m_kerning_pairs;
  const fs::File &m_file;

  void refresh();
  static int to_charset(char ascii);

  void draw_char_on_bitmap(
    const sg_font_char_t &ch,
    Bitmap &dest,
    const Point &point) const;
  int load_char(sg_font_char_t &ch, char c, bool ascii) const;
  sg_font_kerning_pair_t load_kerning(u32 offset) const;
  int load_kerning(u16 first, u16 second) const;
  /*! \endcond */

private:
};

} // namespace ux::sgfx

#endif /* UXAPI_UX_SGFX_FONT_HPP_ */
