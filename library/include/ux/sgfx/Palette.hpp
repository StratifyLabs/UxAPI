// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#ifndef UXAPI_UX_SGFX_PALETTE_HPP
#define UXAPI_UX_SGFX_PALETTE_HPP

#include <var/Data.hpp>
#include <var/StackString.hpp>

#include "sgfx_types.h"

namespace ux::sgfx {

class PaletteFlags {
public:
  enum class BitsPerPixel {
    x1 = 1,
    x2 = 2,
    x4 = 4,
    x8 = 8,
    x16 = 16,
    x24 = 24,
    x32 = 32
  };

  enum class IsAscending { no, yes };

  enum class ColorCount {
    null = 0,
    x1bpp = 2,
    x2bpp = 4,
    x4bpp = 16,
    x8bpp = 256,
    x16bpp = 65536
  };

  enum class PixelFormat {
    null,
    rgb332,
    rgb444,
    rgb565,
    rgb666,
    rgb888,
    rgba8888
  };

  typedef struct MCU_PACK {
    u8 red;
    u8 green;
    u8 blue;
    u8 alpha;
  } palette_color_t;

  static PixelFormat decode_pixel_format(const var::StringView format);
  static u32 bits_per_pixel_format(PixelFormat format);
};

class PaletteColor : public PaletteFlags {
public:
  explicit PaletteColor(const var::StringView hex_code) {
    import_hex_code(hex_code);
  }

  static PaletteColor from_rgb332(const u16 value) {
    PaletteColor result;
    result.m_color.alpha = 0xff;
    result.m_color.red = (value >> 5) & 0x07;
    result.m_color.green = (value >> 2) & 0x07;
    result.m_color.blue = value & 0x03;
    return result;
  }

  static PaletteColor from_rgb444(const u16 value) {
    PaletteColor result;
    result.m_color.alpha = 0xff;
    result.m_color.red = ((value >> 8) & 0x0f) << 4;
    result.m_color.green = ((value >> 4) & 0x0f) << 4;
    result.m_color.blue = (value & 0x0f) << 4;
    return result;
  }

  static PaletteColor from_rgb565(const u16 value) {
    PaletteColor result;
    result.m_color.alpha = 0xff;
    result.m_color.red = ((value >> 11) & 0x1f) << 3;
    result.m_color.green = ((value >> 5) & 0x3f) << 2;
    result.m_color.blue = (value & 0x1f) << 3;
    return result;
  }

  static PaletteColor from_rgb666(const u16 value) {
    PaletteColor result;
    result.m_color.alpha = 0xff;
    result.m_color.red = ((value >> 12) & 0x3f) << 2;
    result.m_color.green = ((value >> 6) & 0x3f) << 2;
    result.m_color.blue = (value & 0x3f) << 2;
    return result;
  }

  static PaletteColor from_rgb888(const u32 value) {
    PaletteColor result;
    result.m_color.alpha = 0xff;
    result.m_color.red = value >> 16;
    result.m_color.green = value >> 8;
    result.m_color.blue = value;
    return result;
  }

  static PaletteColor from_rgb8888(const u32 value) {
    PaletteColor result;
    result.m_color.alpha = value >> 24;
    result.m_color.red = value >> 16;
    result.m_color.green = value >> 8;
    result.m_color.blue = value;
    return result;
  }

  static PaletteColor from_color(sg_color_t c) {
    PaletteColor result;
    result.m_color.alpha = c >> 24;
    result.m_color.red = c >> 16;
    result.m_color.green = c >> 8;
    result.m_color.blue = c;
    return result;
  }

  PaletteColor() : m_color({0}) {}

  PaletteColor(const palette_color_t &color) : m_color(color) {}

  PaletteColor lighter(float a) const;
  PaletteColor darker(float a) const;
  PaletteColor blend(const PaletteColor &a, float ratio) const;

  u32 to(PixelFormat pixel_format) {
    switch (pixel_format) {
    case PixelFormat::null:
      return 0;
    case PixelFormat::rgb332:
      return to_rgb332();
    case PixelFormat::rgb444:
      return to_rgb444();
    case PixelFormat::rgb565:
      return to_rgb565();
    case PixelFormat::rgb666:
      return to_rgb666();
    case PixelFormat::rgb888:
      return to_rgb888();
    case PixelFormat::rgba8888:
      return to_rgba8888();
    }
    return 0;
  }

  PaletteColor operator*(float a) const {
    PaletteColor result;
    result.m_color.alpha = m_color.alpha;
    result.m_color.red = multiply_component(m_color.red, a);
    result.m_color.green = multiply_component(m_color.green, a);
    result.m_color.blue = multiply_component(m_color.blue, a);
    return result;
  }

  PaletteColor &operator*(float a) {
    *this = *this * a;
    return *this;
  }

  var::KeyString to_hex_code() const {
    return var::KeyString().format("#%02x%02x%02x", red(), green(), blue());
  }

  sg_color_t to_pixel_format(PixelFormat pixel_format) const;
  sg_color_t to_rgb332() const;
  sg_color_t to_rgb444() const;
  sg_color_t to_rgb565() const;
  sg_color_t to_rgb666() const;
  sg_color_t to_rgb888() const;
  sg_color_t to_rgba8888() const;

  u8 red() const { return m_color.red; }
  u8 green() const { return m_color.green; }
  u8 blue() const { return m_color.blue; }
  u8 alpha() const { return m_color.alpha; }
  sg_color_t color() const { return to_rgba8888(); }

  u8 get_brightness() const {
    return ((u32)m_color.red + m_color.green + m_color.blue) / 3;
  }

  static var::Vector<PaletteColor> calculate_gradient(
    const PaletteColor &start,
    const PaletteColor &end,
    size_t count);

private:
  palette_color_t m_color;

  PaletteColor adjust(float a) const;
  static u8 adjust_component(u8 value, float a);
  static u8 multiply_component(u8 value, float a);
  static u8 add_component(u8 a, u8 b);
  static u8 subtract_component(u8 a, u8 b);
  static u8 blend_component(u8 a, u8 b, float ratio);

  void import_hex_code(const var::StringView  hex);
};

class Palette : public PaletteFlags {
public:
  Palette();
  Palette &set_pixel_format(PixelFormat value) {
    m_pixel_format = value;
    return *this;
  }

  static bool is_valid(BitsPerPixel bits_per_pixel);

  bool is_valid() const {
    return (colors().count() > 0) && (pixel_format() != PixelFormat::null);
  }

  static ColorCount get_color_count(BitsPerPixel bits_per_pixel);
  BitsPerPixel get_bits_per_pixel() const;

  Palette &set_color_count(ColorCount color_count);

  Palette &assign_color(u32 v, const PaletteColor &color);

  /*! \details Returns the pixel format. */
  PixelFormat pixel_format() const { return m_pixel_format; }

  Palette &create_gradient(
    const PaletteColor &color,
    IsAscending is_ascending = IsAscending(true));

  var::Vector<sg_color_t> &colors() { return m_colors; }
  const var::Vector<sg_color_t> &colors() const { return m_colors; }

  PaletteColor palette_color(size_t offset) const;

private:
  PixelFormat m_pixel_format;
  var::Vector<sg_color_t> m_colors;
};

} // namespace ux::sgfx

namespace printer {
class Printer;
Printer &operator<<(Printer &printer, const ux::sgfx::Palette &palette);
} // namespace printer

#endif // UXAPI_UX_SGFX_PALETTE_HPP
