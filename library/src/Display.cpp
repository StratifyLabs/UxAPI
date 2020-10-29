// Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for
// rights.

#include "ux/Display.hpp"

using namespace ux;

// const Display &Display::write(const sgfx::Bitmap &bitmap) const {}

const Display &Display::set_window(const sgfx::Region &region) const {
  display_attr_t attr;
  attr.o_flags = DISPLAY_FLAG_SET_WINDOW;
  attr.window_x = region.point().x();
  attr.window_y = region.point().y();
  attr.window_width = region.area().width();
  attr.window_height = region.area().height();
  return ioctl(I_DISPLAY_SETATTR, &attr);
}

const Display &Display::set_mode(Mode value) const {
  display_attr_t attr;
  attr.o_flags = DISPLAY_FLAG_SET_MODE;
  if (value == Mode::palette) {
    attr.o_flags |= DISPLAY_FLAG_IS_MODE_PALETTE;
  } else {
    attr.o_flags |= DISPLAY_FLAG_IS_MODE_RAW;
  }
  return ioctl(I_DISPLAY_SETATTR, &attr);
}

const Display &Display::enable() const {
  display_attr_t attributes;
  attributes.o_flags = DISPLAY_FLAG_ENABLE;
  return ioctl(I_DISPLAY_CLEAR, &attributes);
}

const Display &Display::clear() const {
  display_attr_t attributes;
  attributes.o_flags = DISPLAY_FLAG_CLEAR;
  return ioctl(I_DISPLAY_CLEAR, &attributes);
}

const Display &Display::disable() const {
  display_attr_t attributes;
  attributes.o_flags = DISPLAY_FLAG_DISABLE;
  return ioctl(I_DISPLAY_CLEAR, &attributes);
}

const Display &Display::refresh() const {
  return ioctl(I_DISPLAY_REFRESH, nullptr);
}

const Display &Display::wait(const chrono::MicroTime &resolution) const {
  do {
    chrono::wait(resolution);
  } while (is_busy() && is_success());
  return *this;
}

bool Display::is_busy() const {
  return ioctl(I_DISPLAY_ISBUSY, nullptr).return_value() > 0;
}

const Display &Display::set_palette(const sgfx::Palette &palette) const {
  display_palette_t display_palette;
  display_palette.pixel_format = palette.pixel_format();
  display_palette.count = palette.colors().count();
  display_palette.colors = (void *)palette.colors().to_const_void();
  return ioctl(I_DISPLAY_SETPALETTE, &display_palette);
}

sgfx::Palette Display::get_palette() const {
  display_palette_t display_palette;
  ioctl(I_DISPLAY_GETPALETTE, &display_palette);
  API_RETURN_VALUE_IF_ERROR(sgfx::Palette());

  // copy colors from display_palette_t
  sgfx::Palette result;
  result
    .set_pixel_format(static_cast<enum sgfx::Palette::pixel_format>(
      display_palette.pixel_format))
    .set_color_count(
      static_cast<enum sgfx::Palette::color_count>(display_palette.count));

  const sg_color_t *display_palette_colors
    = static_cast<const sg_color_t *>(display_palette.colors);
  for (size_t i = 0; i < result.colors().count(); i++) {
    result.colors().at(i) = display_palette_colors[i];
  }

  return sgfx::Palette(result);
}
