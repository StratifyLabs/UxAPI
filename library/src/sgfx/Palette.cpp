#include <printer/Printer.hpp>
#include <var.hpp>

#include "ux/sgfx/Palette.hpp"

printer::Printer &printer::operator<<(
  printer::Printer &printer,
  const ux::sgfx::Palette &palette) {
  for (size_t i = 0; i < palette.colors().count(); i++) {
    printer.key(
      NumberString(i, "[%04d]").string_view(),
      palette.palette_color(i).to_hex_code().string_view());
  }
  return printer;
}

using namespace ux::sgfx;

PaletteFlags::PixelFormat
PaletteFlags::decode_pixel_format(const var::StringView format) {
  if (format == "rgb332") {
    return PixelFormat::rgb332;
  }
  if (format == "rgb444") {
    return PixelFormat::rgb444;
  }
  if (format == "rgb565") {
    return PixelFormat::rgb565;
  }
  if (format == "rgb666") {
    return PixelFormat::rgb666;
  }
  if (format == "rgb888") {
    return PixelFormat::rgb888;
  }
  if (format == "rgba8888") {
    return PixelFormat::rgba8888;
  }
  return PixelFormat::null;
}

u32 PaletteFlags::bits_per_pixel_format(PixelFormat format) {
  switch (format) {
  case PixelFormat::null:
    return 1;
  case PixelFormat::rgb332:
    return 8;
  case PixelFormat::rgb444:
    return 12;
  case PixelFormat::rgb565:
    return 16;
  case PixelFormat::rgb666:
    return 18;
  case PixelFormat::rgb888:
    return 24;
  case PixelFormat::rgba8888:
    return 32;
  }
  return 1;
}

void PaletteColor::import_hex_code(const var::StringView  hex) {

  if (hex.length() != 7) {
    return;
  }
  m_color.red = hex(StringView::GetSubstring().set_position(1).set_length(2))
                  .to_unsigned_long(StringView::Base::hexadecimal);
  m_color.green = hex(StringView::GetSubstring().set_position(3).set_length(2))
                    .to_unsigned_long(StringView::Base::hexadecimal);
  m_color.blue = hex(StringView::GetSubstring().set_position(4).set_length(2))
                   .to_unsigned_long(StringView::Base::hexadecimal);
}

u8 PaletteColor::multiply_component(u8 value, float a) {
  s32 tmp = value * (a);
  if (tmp > 255) {
    tmp = 255;
  }
  if (tmp < 0) {
    tmp = 0;
  }
  return tmp;
}

u8 PaletteColor::add_component(u8 a, u8 b) {
  u32 tmp = a + b;
  if (tmp > 255) {
    tmp = 255;
  }
  return tmp;
}

u8 PaletteColor::subtract_component(u8 a, u8 b) {
  s32 tmp = a - b;
  if (tmp < 0) {
    tmp = 0;
  }
  return tmp;
}

u8 PaletteColor::blend_component(u8 a, u8 b, float ratio) {
  return add_component(a * ratio, b * (1.0f - ratio));
}

u8 PaletteColor::adjust_component(u8 value, float a) {
  return multiply_component(value, (a > 0.0f ? 1.0f : -1.0f) + a);
}

PaletteColor PaletteColor::blend(const PaletteColor &a, float ratio) const {
  palette_color_t color;
  color.alpha = blend_component(a.alpha(), alpha(), ratio);
  color.red = blend_component(a.red(), red(), ratio);
  color.green = blend_component(a.green(), green(), ratio);
  color.blue = blend_component(a.blue(), blue(), ratio);
  return PaletteColor(color);
}

PaletteColor PaletteColor::adjust(float a) const {
  PaletteColor result(*this);
  result.m_color.red = adjust_component(m_color.red, a);
  result.m_color.green = adjust_component(m_color.green, a);
  result.m_color.blue = adjust_component(m_color.blue, a);
  return result;
}

PaletteColor PaletteColor::lighter(float a) const { return adjust(a); }

PaletteColor PaletteColor::darker(float a) const { return adjust(-1.0 * a); }

sg_color_t PaletteColor::to_pixel_format(PixelFormat pixel_format) const {
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

sg_color_t PaletteColor::to_rgb332() const {
  u8 rgb = (m_color.red & 0xe0);      // top 3
  rgb |= (m_color.green & 0xe0) >> 3; // top 3 green, shifted 3
  rgb |= (m_color.blue & 0xc0) >> 6;  // top 2 blue, shifted 6
  return rgb;
}

sg_color_t PaletteColor::to_rgb565() const {
  u16 rgb = (m_color.red & 0xf8) << 8;
  rgb |= (m_color.green & 0xfc) << 3;
  rgb |= (m_color.blue & 0xf8) >> 3;
  return rgb;
}

sg_color_t PaletteColor::to_rgb444() const {
  u16 rgb = (m_color.red & 0xf0) << 4;
  rgb |= (m_color.green & 0xf0);
  rgb |= (m_color.blue & 0xf0) >> 4;
  return rgb;
}

sg_color_t PaletteColor::to_rgb666() const {
  u32 rgb = (m_color.red & 0xfc) << 10;
  rgb |= (m_color.green & 0xfc) << 4;
  rgb |= (m_color.blue & 0xfc) >> 2;
  return rgb;
}

sg_color_t PaletteColor::to_rgb888() const {
  u32 result;
  result = (m_color.red << 16) | (m_color.green << 8) | m_color.blue;
  return result;
}

sg_color_t PaletteColor::to_rgba8888() const {
  u32 result;
  result = (m_color.alpha << 24) | (m_color.red << 16) | (m_color.green << 8)
           | m_color.blue;
  return result;
}

var::Vector<PaletteColor> PaletteColor::calculate_gradient(
  const PaletteColor &start,
  const PaletteColor &end,
  size_t count) {
  var::Vector<PaletteColor> result(count);
  for (size_t i = 0; i < result.count(); i++) {
    float ratio = (i * 1.0f) / (result.count() - 1);
    result.at(i) = start.blend(end, ratio);
  }
  return result;
}

Palette::Palette() { m_pixel_format = PixelFormat::null; }

PaletteFlags::ColorCount Palette::get_color_count(BitsPerPixel bits_per_pixel) {
  switch (bits_per_pixel) {
  case BitsPerPixel::x1:
    return ColorCount::x1bpp;
  case BitsPerPixel::x2:
    return ColorCount::x2bpp;
  case BitsPerPixel::x4:
    return ColorCount::x4bpp;
  case BitsPerPixel::x8:
    return ColorCount::x8bpp;
  case BitsPerPixel::x16:
    return ColorCount::x16bpp;
  default:
    break;
  }
  return ColorCount::null;
}

Palette::BitsPerPixel Palette::get_bits_per_pixel() const {
  u32 count = colors().count();
  if (count == 2) {
    return BitsPerPixel::x1;
  }
  if (count == 4) {
    return BitsPerPixel::x2;
  }
  if (count == 16) {
    return BitsPerPixel::x4;
  }
  if (count == 256) {
    return BitsPerPixel::x8;
  }
  if (count == 65536) {
    return BitsPerPixel::x16;
  }
  return BitsPerPixel::x1;
}

Palette &Palette::set_color_count(ColorCount color_count) {
  m_colors = var::Vector<sg_color_t>();
  m_colors.resize(static_cast<size_t>(color_count));
  return *this;
}

Palette &Palette::assign_color(u32 v, const PaletteColor &color) {
  if (colors().count() == 0) {
    return *this;
  }
  u32 offset = v % colors().count();
  sg_color_t c = color.to_pixel_format(pixel_format());
  m_colors.at(offset) = c;
  return *this;
}

PaletteColor Palette::palette_color(size_t offset) const {
  if (offset < colors().count()) {
    sg_color_t color = colors().at(offset);
    switch (pixel_format()) {
    case PixelFormat::null:
      return PaletteColor();
    case PixelFormat::rgb332:
      return PaletteColor::from_rgb332(color);
    case PixelFormat::rgb444:
      return PaletteColor::from_rgb444(color);
    case PixelFormat::rgb565:
      return PaletteColor::from_rgb565(color);
    case PixelFormat::rgb666:
      return PaletteColor::from_rgb666(color);
    case PixelFormat::rgb888:
      return PaletteColor::from_rgb888(color);
    case PixelFormat::rgba8888:
      return PaletteColor::from_rgb8888(color);
    }
  }
  return PaletteColor();
}

Palette &
Palette::create_gradient(const PaletteColor &color, IsAscending is_ascending) {

  if (colors().count() == 0) {
    return *this;
  }

  const float max = colors().count() - 1.0f;
  for (u32 i = 0; i < colors().count(); i++) {
    float scale;
    if (is_ascending == IsAscending::yes) {
      scale = i * 1.0f / max;
    } else {
      scale = (max - i) * 1.0f / max;
    }

    assign_color(i, color * scale);
  }

  return *this;
}

bool Palette::is_valid(BitsPerPixel bits_per_pixel) {
  switch (bits_per_pixel) {
  case BitsPerPixel::x1:
  case BitsPerPixel::x2:
  case BitsPerPixel::x4:
  case BitsPerPixel::x8:
  case BitsPerPixel::x16:
  case BitsPerPixel::x24:
  case BitsPerPixel::x32:
    return true;
  default:
    return false;
  }
}
