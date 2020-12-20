// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#include "ux/fmt/Bmp.hpp"
#include <cstdlib>
#include <cstring>

using namespace ux::sgfx;
using namespace ux::fmt;

using namespace fs;

Bmp::Bmp() {
  m_dib = {0};
  m_offset = 0;
}

Bmp::Bmp(const fs::FileObject *file_object) : m_file_object(file_object) {
  API_ASSERT(file_object != nullptr);
  load();
}

void Bmp::load() {
  bmp_header_t hdr;

  m_dib.width = -1;
  m_dib.height = -1;
  m_dib.bits_per_pixel = 0;
  m_file_object->read(var::View(hdr)).read(var::View(m_dib)).seek(hdr.offset);

  m_offset = hdr.offset;
}

Bmp::Bmp(const Construct &options) : m_file_object(options.file_object()) {

  API_ASSERT(m_file_object != nullptr);

  bmp_header_t hdr;

  const int bits_per_pixel = static_cast<int>(options.bits_per_pixel());

  hdr.size = sizeof(hdr) + sizeof(m_dib)
             + (options.width() * options.height() * bits_per_pixel + 7) / 8;

  hdr.offset = sizeof(hdr) + sizeof(m_dib);
  hdr.signature = signature();
  hdr.resd1 = 0;
  hdr.resd2 = 0;

  m_dib.bits_per_pixel = bits_per_pixel;
  m_dib.hdr_size = sizeof(m_dib);
  m_dib.height = options.height();
  m_dib.width = options.width();
  m_dib.planes = options.plane_count();

  m_file_object->seek(0).write(var::View(hdr)).write(var::View(m_dib));
}

unsigned int Bmp::calculate_row_size() const {
  return (((m_dib.bits_per_pixel / 8) * m_dib.width + 3) / 4) * 4;
}

const Bmp &Bmp::seek_row(s32 y) const {
  if (m_dib.height > 0) {
    m_file_object->seek(m_offset + calc_row_size() * (m_dib.height - (y + 1)));
    return *this;
  }

  m_file_object->seek(m_offset + calc_row_size() * y);
  return *this;
}

bool Bmp::to_mono(const var::View pixel, u8 threshold) const {

  u32 avg = 0;
  for (u32 i = 0; i < pixel.size(); i++) {
    avg += pixel.to_const_u8()[i];
  }
  avg = avg / 8;
  if (avg > threshold) {
    return true;
  }

  return false;
}

const Bmp &Bmp::read_pixel(var::View pixel) const {

  m_file_object->read(pixel);
  return *this;
}

void Bmp::save(
  const fs::FileObject &file,
  const sgfx::Bitmap &bitmap,
  const sgfx::Palette &palette) {

  Bmp bmp(Construct()
            .set_file_object(&file)
            .set_width(bitmap.width())
            .set_height(bitmap.height())
            .set_plane_count(1)
            .set_bits_per_pixel(BitsPerPixel::x24));

  API_RETURN_IF_ERROR();

  for (sg_int_t y = bitmap.height() - 1; y >= 0; y--) {
    var::Vector<u8> row(bmp.calculate_row_size());
    for (sg_int_t x = 0; x < bitmap.width(); x++) {
      sg_color_t color = bitmap.get_pixel(sgfx::Point(x, y));
      PaletteColor palette_color = palette.palette_color(color);

      row.at(x * 3) = palette_color.blue();
      row.at(x * 3 + 1) = palette_color.green();
      row.at(x * 3 + 2) = palette_color.red();
    }

    bmp.m_file_object->write(row);
  }
}

ux::sgfx::BitmapData Bmp::convert_to_bitmap(BitsPerPixel bpp) {
  s32 i, j;
  int avg;
  u8 pixel[3];
  var::View view_pixel(pixel);

  sgfx::BitmapData result(Area(width(), height()), bpp);

  for (j = 0; j < height(); j++) {
    seek_row(j);

    for (i = 0; i < width(); i++) {

      read_pixel(view_pixel);
      avg = (pixel[0] + pixel[1] + pixel[2]) / 3;

      sg_color_t bitmap_color;
      bitmap_color = avg * (result.color_count() - 1) / 255;
      if ((bitmap_color == 0) && (avg > 0)) {
        bitmap_color = 1;
      }
      // where is brightness in index
      result.set_pen(Pen().set_color(bitmap_color)).draw_pixel(Point(i, j));
    }
  }

  return result;
}
