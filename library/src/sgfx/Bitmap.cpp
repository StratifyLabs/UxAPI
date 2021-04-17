// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#include <stdlib.h>

#include <printer/Printer.hpp>
#include <var.hpp>

#include "fs/File.hpp"
#include "ux/sgfx/Bitmap.hpp"
#include "ux/sgfx/Cursor.hpp"

printer::Printer &
printer::operator<<(printer::Printer &printer, const ux::sgfx::Bitmap &a) {
  sg_size_t i, j;

  sg_color_t color;
  sg_cursor_t y_cursor;
  sg_cursor_t x_cursor;

  ux::sgfx::Bitmap::api()->cursor_set(&y_cursor, a.bmap(), sg_point(0, 0));

  var::String line;
  line += " ";
  for (j = 0; j < a.bmap()->area.width; j++) {
    if (j % 10) {
      line += var::NumberString(j % 10);
    } else {
      line += (" ");
    }
  }

  printer.key("lines    ", line);
  line.clear();
  for (j = 0; j < a.bmap()->area.width; j++) {
    line += ("-");
  }
  line += ("--");
  printer.key("start    ", line);

  for (i = 0; i < a.bmap()->area.height; i++) {
    sg_cursor_copy(&x_cursor, &y_cursor);

    line.clear();
    line += ("|");
    for (j = 0; j < a.bmap()->area.width; j++) {
      color = ux::sgfx::Bitmap::api()->cursor_get_pixel(&x_cursor);
      line
        += Printer::get_bitmap_pixel_character(color, a.bmap()->bits_per_pixel);
    }
    line += ("|");
    printer.key(var::NumberString().format("line-%04d", i), line);
    ux::sgfx::Bitmap::api()->cursor_inc_y(&y_cursor);
  }
  line.clear();
  for (j = 0; j < a.bmap()->area.width; j++) {
    line += ("-");
  }
  line += ("--");
  printer.key("lines end", line);

  return printer;
}

printer::Printer &
printer::operator<<(printer::Printer &printer, const ux::sgfx::Point &a) {
  printer.key("x", NumberString(a.x()));
  printer.key("y", NumberString(a.y()));
  return printer;
}

printer::Printer &
printer::operator<<(printer::Printer &printer, const ux::sgfx::Region &a) {
  printer.key("x", NumberString(a.point().x()));
  printer.key("y", NumberString(a.point().y()));
  printer.key("width", NumberString(a.area().width()));
  printer.key("height", NumberString(a.area().height()));
  return printer;
}

printer::Printer &
printer::operator<<(printer::Printer &printer, const ux::sgfx::Area &a) {
  printer.key("width", NumberString(a.width()));
  printer.key("height", NumberString(a.height()));
  return printer;
}

printer::Printer &
printer::operator<<(printer::Printer &printer, const ux::sgfx::Pen &a) {
  printer.key("color", NumberString(a.color()));
  printer.key("thickness", NumberString(a.color()));
  printer.key("o_flags", NumberString(static_cast<int>(a.flags()), "0x%x"));
  printer.key_bool("solid", (a.is_solid()));
  printer.key_bool("invert", (a.is_invert()));
  printer.key_bool("erase", (a.is_erase()));
  printer.key_bool("blend", (a.is_blend()));
  printer.key_bool("fill", (a.is_fill()));
  return printer;
}

using namespace ux::sgfx;

int AntiAliasFilter::initialize(var::Array<u8, 8> contrast_map) {
  return Bitmap::api()->antialias_filter_init(&m_filter, contrast_map.data());
}

BitmapData &BitmapData::load(const fs::FileObject &file) {
  sg_bmap_header_t hdr = {0};

  file.read(View(hdr));

  if (
    (hdr.version != api()->sos_api.version)
    || (hdr.bits_per_pixel != api()->bits_per_pixel)) {
    API_RETURN_VALUE_ASSIGN_ERROR(*this, "", EINVAL);
  }

  resize(
    Area(hdr.width, hdr.height),
    static_cast<BitsPerPixel>(hdr.bits_per_pixel));

  file.read(view());

  return *this;
}

BitmapData &BitmapData::resize(const Area &area, BitsPerPixel bits_per_pixel) {
  sg_bmap_t bmap = {0};
  bmap.bits_per_pixel = static_cast<u8>(bits_per_pixel);

  // use sgfx to get the size of the bitmap
  const size_t size = api()->calc_bmap_size(&bmap, area.area());
  m_data.resize(size);

  if (is_success()) {
    initialize_members(view(), area, bits_per_pixel);
  }

  return *this;
}

Area BitmapData::load_area(const fs::FileObject &file) {
  sg_bmap_header_t hdr;
  file.read(View(hdr));
  if (
    (hdr.version != api()->sos_api.version)
    || (hdr.bits_per_pixel != api()->bits_per_pixel)) {
    return Area();
  }

  return Area(hdr.width, hdr.height);
}

u32 Bitmap::color_count() const {
  return 1 << static_cast<u8>(bits_per_pixel());
}

Region Bitmap::get_viewable_region() const {
  const Point point = Point(margin_left(), margin_top());
  const Area area(
    width() - margin_left() - margin_right(),
    height() - margin_top() - margin_bottom());
  return Region(point, area);
}

int Bitmap::set_internal_bits_per_pixel(BitsPerPixel bpp) {
  // api bpp of zero means the api supports variable bpp values
  if (api()->bits_per_pixel == 0) {
    switch (bpp) {
    case BitsPerPixel::x1:
    case BitsPerPixel::x2:
    case BitsPerPixel::x4:
    case BitsPerPixel::x8:
    case BitsPerPixel::x16:
    case BitsPerPixel::x24:
    case BitsPerPixel::x32:
      m_bmap.bits_per_pixel = static_cast<u8>(bpp);
      return 0;
    }
  } else {
    // bpp is fixed by the sgfx library build
    m_bmap.bits_per_pixel = api()->bits_per_pixel;
  }
  return -1;
}

void Bitmap::initialize_members(
  var::View view,
  const Area &area,
  BitsPerPixel bits_per_pixel) {

  // ensure api() is valid
  if (api().is_valid() == false) {
    exit_fatal("sgfx api not available");
  }

  set_internal_bits_per_pixel(bits_per_pixel);

  api()->bmap_set_data(
    &m_bmap,
    view.to<sg_bmap_data_t>(),
    area,
    m_bmap.bits_per_pixel);

  // verify the view size is acceptable for the bitmap
  if (view.to_void() != nullptr) {
    const size_t minimum_size = api()->calc_bmap_size(&m_bmap, area.area());
    API_ASSERT(view.size() >= minimum_size);
  }

  m_bmap.margin_bottom_right.area = 0;
  m_bmap.margin_top_left.area = 0;
  m_bmap.pen.thickness = 1;
  m_bmap.pen.o_flags = 0;
  m_bmap.pen.color = 65535;
}

var::View Bitmap::to_view() {
  return var::View(m_bmap.data, api()->calc_bmap_size(&m_bmap, m_bmap.area));
}

const var::View Bitmap::to_view() const {
  return var::View(m_bmap.data, api()->calc_bmap_size(&m_bmap, m_bmap.area));
}

Bitmap::Bitmap(var::View view, const Area &area, BitsPerPixel bits_per_pixel) {

  set_internal_bits_per_pixel(bits_per_pixel);

  api()->bmap_set_data(
    &m_bmap,
    view.to<sg_bmap_data_t>(),
    area,
    m_bmap.bits_per_pixel);

  // verify the view size is acceptable for the bitmap
  if (view.to_void() != nullptr) {
    const size_t minimum_size = api()->calc_bmap_size(&m_bmap, area.area());
    API_ASSERT(view.size() >= minimum_size);
  }
}

Bitmap::Bitmap() { m_bmap = {0}; }

Point Bitmap::center() const { return Point(width() / 2, height() / 2); }

const sg_bmap_data_t *Bitmap::bmap_data(const Point &p) const {
  return api()->bmap_data(bmap(), p);
}

sg_bmap_data_t *Bitmap::bmap_data(const Point &p) {
  return api()->bmap_data(bmap(), p);
}

const Bitmap &Bitmap::save(const fs::File &file) const {
  sg_bmap_header_t hdr;
  hdr.width = width();
  hdr.height = height();
  hdr.size = calculate_size();
  hdr.bits_per_pixel = api()->bits_per_pixel;
  hdr.version = api()->sos_api.version;
  file.write(View(hdr)).write(m_bmap.data, calculate_size());
  return *this;
}

Region Bitmap::calculate_active_region() const {
  Region result;
  sg_point_t point;
  sg_point_t top_left;
  sg_point_t bottom_right;

  top_left.x = width();
  top_left.y = height();
  bottom_right.x = 0;
  bottom_right.y = 0;
  bool is_blank = true;

  for (point.y = 0; point.y < height(); point.y++) {
    bool is_row_blank = true;
    for (point.x = 0; point.x < width(); point.x++) {

      if (get_pixel(point)) {
        is_blank = false;
        is_row_blank = false;

        if (point.x < top_left.x) {
          top_left.x = point.x;
        }

        if (point.x > bottom_right.x) {
          bottom_right.x = point.x;
        }
      }
    }

    if (!is_row_blank) {
      if (point.y < top_left.y) {
        top_left.y = point.y;
      }

      if (point.y > bottom_right.y) {
        bottom_right.y = point.y;
      }
    }
  }

  if (is_blank) {
    top_left.x = width() / 2;
    top_left.y = width() / 2;
    bottom_right.x = width() / 2;
    bottom_right.y = width() / 2;
  }

  result.set_region(top_left, bottom_right);

  return result;
}

bool Bitmap::is_empty(const Region &region) const {
  Cursor x_cursor;
  Cursor y_cursor(*this, region.point());
  for (u32 h = 0; h < region.area().height(); h++) {
    x_cursor = y_cursor;
    for (u32 w = 0; w < region.area().width(); w++) {
      // get pixel increments the cursor
      if (x_cursor.get_pixel() != 0) {
        return false;
      }
    }
    y_cursor.increment_y();
  }
  return true;
}

Bitmap &Bitmap::downsample(const Bitmap &source) {

  API_ASSERT(width() != 0);
  API_ASSERT(height() != 0);

  Cursor cursor_x, cursor_y;
  Area factor(source.width() / width(), source.height() / height());

  cursor_y.set_bitmap(*this);
  for (sg_int_t y = 0; y <= source.height() - factor.height() / 2;
       y += factor.height()) {
    cursor_x = cursor_y;

    for (sg_int_t x = 0; x <= source.width() - factor.width() / 2;
         x += factor.width()) {
      Region region(Point(x, y), factor);

#if defined NOT_BUILDING
      sample.clear();
      sample.draw_sub_bitmap(Point(), source, region);

      u32 color = sample.calculate_color_sum();
      if (color >= factor.calculate_area() / 2) {
        bmap()->pen.color = static_cast<u32>(-1);
      } else {
        bmap()->pen.color = 0;
      }
#endif

      cursor_x.draw_pixel();
    }

    cursor_y.increment_y();
  }
  return *this;
}

sg_color_t Bitmap::calculate_color_sum() {
  sg_color_t color = 0;
  Cursor cursor_y, cursor_x;
  cursor_y.set_bitmap(*this);
  for (sg_size_t y = 0; y < height(); y++) {
    cursor_x = cursor_y;
    for (sg_size_t x = 0; x < width(); x++) {
      color += cursor_x.get_pixel();
    }
    cursor_y.increment_y();
  }
  return color;
}

Region Bitmap::fill_empty_region(Area area) {
  sg_point_t point;
  Region region(Point(0, 0), area);

  for (point.y = 0; point.y < height() - area.height(); point.y++) {
    for (point.x = 0; point.x < width() - area.width(); point.x++) {
      region.set_point(point);
      if (is_empty(region)) {
        draw_rectangle(region);
        return region;
      }
    }
  }

  return Region();
}
