/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#ifndef UXAPI_UX_SGFX_THEME_HPP
#define UXAPI_UX_SGFX_THEME_HPP

#include "../Display.hpp"
#include "Bitmap.hpp"
#include "Palette.hpp"

namespace ux::sgfx {

class Theme : public PaletteFlags {
public:
  using BitsPerPixel = Bitmap::BitsPerPixel;

  Theme(fs::File &color_file);

  enum class Style {
    dark,
    light,
    brand_primary,
    brand_secondary,
    info,
    success,
    warning,
    danger,
    outline_dark,
    outline_light,
    outline_brand_primary,
    outline_brand_secondary,
    outline_info,
    outline_success,
    outline_warning,
    outline_danger,
    first_style = dark,
    last_style = outline_danger
  };

  enum class State {
    default_,
    highlighted,
    disabled,
    first_state = default_,
    last_state = disabled
  };

  Theme &set_bits_per_pixel(u8 bits_per_pixel);
  BitsPerPixel bits_per_pixel() const {
    return static_cast<BitsPerPixel>(m_header.bits_per_pixel);
  }

  Theme &set_primary_font_name(const var::StringView  name) {
    m_primary_font_name = name;
    return *this;
  }

  Theme &set_primary_icon_font_name(const var::StringView  name) {
    m_primary_icon_font_name = name;
    return *this;
  }

  const var::StringView  primary_font_name() const {
    return m_primary_font_name.string_view();
  }

  const var::StringView  primary_icon_font_name() const {
    return m_primary_icon_font_name.string_view();
  }

  u16 color_count() const { return m_color_count; }

  Theme &create(
    const var::StringView  path,
    fs::File::IsOverwrite is_overwrite,
    BitsPerPixel bits_per_pixel,
    PixelFormat pixel_format);
  Theme &load(const var::StringView path);

  size_t calculate_color_offset(Style style, State state) const;

  Theme &write_palette(Style style, State state, const Palette &palette);

  Palette read_palette(Style style, State state) const;

  const Theme &
  set_display_palette(const Display &display, Style style, State state) const;

  static var::StringView get_state_name(State value);
  static var::StringView get_style_name(Style value);

  const chrono::MicroTime &button_hold_duration() const {
    return m_button_hold_duration;
  }

  sg_color_t background_color() const { return 0; }
  sg_color_t color() const { return 1; }
  sg_color_t text_color() const;
  sg_color_t border_color() const;

  AntiAliasFilter &antialias_filter() { return m_antialias_filter; }

  const AntiAliasFilter &antialias_filter() const { return m_antialias_filter; }

private:
  typedef struct MCU_PACK {
    u16 version;
    u8 bits_per_pixel;
    u8 pixel_format;
  } header_t;

  // holds the RGB values for all styles and states

  fs::File &m_color_file;
  AntiAliasFilter m_antialias_filter;
  var::NameString m_primary_font_name;
  var::NameString m_primary_icon_font_name;
  chrono::MicroTime m_button_hold_duration = 750_milliseconds;

  header_t m_header = {0};
  u16 m_color_count = 0;

  size_t header_color_count() const { return 1 << (m_header.bits_per_pixel); }
};

} // namespace ux::sgfx

namespace printer {
class Printer;
Printer &operator<<(Printer &printer, const ux::sgfx::Theme &theme);
} // namespace printer

#endif // UXAPI_UX_SGFX_THEME_HPP
