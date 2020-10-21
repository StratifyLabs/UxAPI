/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#include <json/Json.hpp>

#include "fs/File.hpp"
#include "ux/sgfx/Theme.hpp"

#define VERSION 0x0308

using namespace ux::sgfx;

Theme::Theme(fs::File &color_file) : m_color_file(color_file) {}

var::StringView Theme::get_style_name(Style value) {
  switch (value) {
  case Style::dark:
    return "dark";
  case Style::light:
    return "light";
  case Style::brand_primary:
    return "brandPrimary";
  case Style::brand_secondary:
    return "brandSecondary";
  case Style::info:
    return "info";
  case Style::success:
    return "success";
  case Style::warning:
    return "warning";
  case Style::danger:
    return "danger";
  case Style::outline_dark:
    return "outlineDark";
  case Style::outline_light:
    return "outlineLight";
  case Style::outline_brand_primary:
    return "outlineBrandPrimary";
  case Style::outline_brand_secondary:
    return "outlineBrandSecondary";
  case Style::outline_info:
    return "outlineInfo";
  case Style::outline_success:
    return "outlineSuccess";
  case Style::outline_warning:
    return "outlineWarning";
  case Style::outline_danger:
    return "outlineDanger";
  }
  return "";
}

sg_color_t Theme::text_color() const {
  if (bits_per_pixel() == 1) {
    return 1;
  }
  return (1 << bits_per_pixel()) - 2;
}

sg_color_t Theme::border_color() const {
  if (bits_per_pixel() == 1) {
    return 0;
  }
  return (1 << bits_per_pixel()) - 1;
}

var::StringView Theme::get_state_name(State value) {
  switch (value) {
  case State::default_:
    return "default";
  case State::disabled:
    return "disabled";
  case State::highlighted:
    return "highlighted";
  }
  return "";
}

size_t Theme::calculate_color_offset(Style style, State state) const {
  return (static_cast<int>(style) * m_color_count
            * (static_cast<int>(State::last_state) + 1)
          + (static_cast<int>(state) * m_color_count))
           * sizeof(sg_color_t)
         + sizeof(header_t);
}

Theme &Theme::set_bits_per_pixel(u8 bits_per_pixel) {
  m_header.bits_per_pixel = bits_per_pixel;
  m_color_count = header_color_count();
  return *this;
}

Theme &Theme::write_palette(Style style, State state, const Palette &palette) {
  API_ASSERT(m_color_count == palette.colors().count());
  m_color_file.seek(calculate_color_offset(style, state))
    .write(palette.colors());
  return *this;
}

Palette Theme::read_palette(Style style, State state) const {
  Palette result;
  result
    .set_pixel_format(
      static_cast<enum Palette::pixel_format>(m_header.pixel_format))
    .set_color_count(static_cast<enum Palette::color_count>(m_color_count));
  const int offset = calculate_color_offset(style, state);
  m_color_file.seek(offset).read(result.colors());
  return result;
}

Theme &Theme::load(const var::String &path) {
  m_color_file = std::move(fs::File(path).read(var::View(m_header)));
  m_color_count = header_color_count();
  return *this;
}

Theme &Theme::create(
  const var::StringView path,
  fs::File::IsOverwrite is_overwrite,
  BitsPerPixel bits_per_pixel,
  enum pixel_format pixel_format) {

  m_header.version = VERSION;
  m_header.bits_per_pixel = static_cast<u8>(bits_per_pixel);
  m_header.pixel_format = pixel_format;
  m_color_count = 0;

  m_color_file
    = std::move(fs::File(is_overwrite, path).write(var::View(m_header)));

  m_color_count = header_color_count();

  return *this;
}

const Theme &Theme::set_display_palette(
  const Display &display,
  Style style,
  State state) const {
  display.set_palette(read_palette(style, state));
  return *this;
}
