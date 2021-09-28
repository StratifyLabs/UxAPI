// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#ifndef UXAPI_UX_SGFX_BITMAP_HPP_
#define UXAPI_UX_SGFX_BITMAP_HPP_

#include <chrono/MicroTime.hpp>
#include <fs/File.hpp>
#include <var/Data.hpp>
#include <var/Vector.hpp>

#include "Api.hpp"

#include "Palette.hpp"
#include "Pen.hpp"
#include "Region.hpp"

namespace ux::sgfx {

class BitmapFlags {
public:
  using BitsPerPixel = PaletteFlags::BitsPerPixel;
};

class AntiAliasFilter {
public:
  int initialize(var::Array<u8, 8> contrast_map);

  const sg_antialias_filter_t *filter() const { return &m_filter; }

private:
  sg_antialias_filter_t m_filter
#if defined __win32

#else
    = {}
#endif
    ;
};

/*! \brief Bitmap Class
 * \details This class implements a bitmap and is
 * powered by the sgfx library.
 */
class Bitmap : public Api, public BitmapFlags {
public:
  Bitmap();

  Bitmap(var::View view, const Area &area, BitsPerPixel bits_per_pixel);

  BitsPerPixel bits_per_pixel() const {
    return static_cast<BitsPerPixel>(m_bmap.bits_per_pixel);
  }

  Bitmap &set_pen(const Pen &pen) {
    m_bmap.pen = pen.pen();
    return *this;
  }

  Pen get_pen() const { return Pen(m_bmap.pen); }

  Region get_viewable_region() const;

  const Bitmap &save(const fs::File &file) const;

  Bitmap get_bitmap(const Region &region);

  /*! \details Returns the number of bytes used to store a Bitmap of specified
   * size
   *
   * @param w Width used for calculation
   * @param h Height used for calculation
   */
  u32 calculate_size(const Area &area) const {
    return api()->calc_bmap_size(bmap(), area);
  }

  /*! \details Returns the number of bytes used to store the Bitmap. */
  u32 calculate_size() const { return calculate_size(area()); }

  Point center() const;

  /*! \details Returns the maximum x value. */
  sg_int_t x_max() const { return width() - 1; }
  /*! \details Returns the maximum y value. */
  sg_int_t y_max() const { return height() - 1; }

  static Area load_area(const var::StringView  path);

  const Bitmap &transform_flip_x() const {
    api()->transform_flip_x(bmap());
    return *this;
  }
  const Bitmap &transform_flip_y() const {
    api()->transform_flip_y(bmap());
    return *this;
  }
  const Bitmap &transform_flip_xy() const {
    api()->transform_flip_xy(bmap());
    return *this;
  }

  /*! \details Performs a shift operation on an area of the bitmap.
   *
   * @param shift The amount to shift in each direction
   * @param region The region to shift
   *
   * The shifting must respect the dimensions of the bitmap. If you want to
   * shift the entire bitmap to the left 8 pixels, do this:
   *
   * \code
   * 	Bitmap bmap(64,64);
   * 	bmap.transform_shift(sg_point(-8,0), sg_point(8,0), sg_dim(bmap.width()
   * - 8, bmap.height)); \endcode
   *
   *
   */
  Bitmap &transform_shift(sg_point_t shift, const sg_region_t &region) {
    api()->transform_shift(bmap(), shift, &region);
    return *this;
  }
  Bitmap &transform_shift(sg_point_t shift, sg_point_t p, sg_area_t d) {
    transform_shift(shift, Region(p, d));
    return *this;
  }

  /*! \details Gets the value of the pixel at the specified point.
   *
   * @param p The point to get the pixel color
   * @return The color of the pixel at \a p
   */
  sg_color_t get_pixel(const Point &p) const {
    return api()->get_pixel(bmap(), p);
  }

  /*! \details Draws a pixel at the specified point.
   *
   * @param p The point where to draw the pixel
   *
   * The color of the pixel is determined by the pen settings.
   *
   * \sa set_pen_color()
   */
  const Bitmap &draw_pixel(const Point &p) const {
    api()->draw_pixel(bmap(), p);
    return *this;
  }

  /*! \details Draws a line on the bitmap.
   *
   * @param p1 Starting point
   * @param p2 Ending point
   *
   * The bitmap's pen will determine the color, thickness, and drawing mode.
   *
   */
  const Bitmap &draw_line(const Point &p1, const Point &p2) const {
    api()->draw_line(bmap(), p1, p2);
    return *this;
  }

  const Bitmap &draw_quadratic_bezier(
    const Point &p1,
    const Point &p2,
    const Point &p3,
    sg_point_t *corners = 0) const {
    api()->draw_quadratic_bezier(bmap(), p1, p2, p3, corners);
    return *this;
  }

  const Bitmap &draw_cubic_bezier(
    const Point &p1,
    const Point &p2,
    const Point &p3,
    const Point &p4,
    sg_point_t *corners = 0) const {
    api()->draw_cubic_bezier(bmap(), p1, p2, p3, p4, corners);
    return *this;
  }

  const Bitmap &draw_arc(
    const Region &region,
    s16 start,
    s16 end,
    s16 rotation = 0,
    sg_point_t *corners = 0) const {
    api()->draw_arc(bmap(), &region.region(), start, end, rotation, corners);
    return *this;
  }

  const Bitmap &
  draw_arc(const Point &p, const Area &d, s16 start, s16 end, s16 rotation = 0)
    const {
    draw_arc(Region(p, d), start, end, rotation);
    return *this;
  }

  /*! \details Draws a rectangle on the bitmap.
   *
   * @param region The region to draw in
   *
   * The bitmap's pen color and drawing mode will affect how the rectangle is
   * drawn. This method affects every pixel in the rectangle not just the
   * border.
   */
  const Bitmap &draw_rectangle(const Region &region) const {
    api()->draw_rectangle(bmap(), &region.region());
    return *this;
  }

  Bitmap &draw_rectangle(const Region &region) {
    return API_CONST_CAST_SELF(Bitmap, draw_rectangle, region);
  }

  /*! \details Pours an area on the bitmap.
   *
   * @param point Center of the pour
   * @param bounds Bounds for the pour
   *
   * The pour will seek boundaries going outward until it hits
   * a non-zero color or hits the bounding box.
   */
  const Bitmap &draw_pour(const Point &point, const Region &bounds) const {
    api()->draw_pour(bmap(), point, &bounds.region());
    return *this;
  }

  /*! \details This function sets the pixels in a bitmap
   * based on the pixels of the source bitmap
   *
   * @param p_dest The point in the destination bitmap of the top left corner of
   * \a bitmap
   * @param src The source bitmap
   * @return Zero on success
   */
  const Bitmap &draw_bitmap(const Point &p_dest, const Bitmap &src) const {
    api()->draw_bitmap(bmap(), p_dest, src.bmap());
    return *this;
  }

  const Bitmap &apply_antialias_filter(
    const AntiAliasFilter &filter,
    const Region &bounds) const {
    api()->antialias_filter_apply(bmap(), filter.filter(), bounds);
    return *this;
  }

  /*!
   * \details Draws the source bitmap by down sampling by factor
   * \param source The bitmap to draw
   * \param factor number to downsample by with different values for width and
   * height (2 means have the size of source)
   *
   *
   */
  Bitmap &downsample(const Bitmap &source);

  /*! \details This function draws a pattern on the bitmap.
   *
   * @param region The region to draw the pattern in
   * @param odd_pattern The odd pattern as a 1bpp bitmask (e.g. 0xAA)
   * @param even_pattern The even pattern as a 1bpp bitmask (e.g. 0x55)
   * @param pattern_height The pixel height of alternating pixels
   */
  const Bitmap &draw_pattern(
    const Region &region,
    sg_bmap_data_t odd_pattern,
    sg_bmap_data_t even_pattern,
    sg_size_t pattern_height) const {
    api()->draw_pattern(
      bmap(),
      &region.region(),
      odd_pattern,
      even_pattern,
      pattern_height);
    return *this;
  }

  /*! \details This function draws a subset of
   * the source bitmap on the destination bitmap.
   *
   * @param p_dest The point in the destination bitmap to start setting pixels
   * @param src The source bitmap
   * @param region_src The regions of the source bitmap to draw
   * @return Zero on success
   */
  const Bitmap &draw_sub_bitmap(
    const Point &destination_point,
    const Bitmap &source_bitmap,
    const Region &source_region) const {
    api()->draw_sub_bitmap(
      bmap(),
      destination_point,
      source_bitmap.bmap(),
      &source_region.region());
    return *this;
  }

  Region calculate_active_region() const;

  Bitmap &invert_rectangle(const Region &region) {
    m_bmap.pen.o_flags = SG_PEN_FLAG_IS_INVERT;
    m_bmap.pen.color = 0xffffffff;
    api()->draw_rectangle(bmap(), &region.region());
    return *this;
  }
  Bitmap &invert() { return invert_rectangle(region()); }

  Region fill_empty_region(Area area);

  Bitmap &clear_rectangle(const Region &region) {
    m_bmap.pen.o_flags = SG_PEN_FLAG_IS_ERASE;
    m_bmap.pen.color = 0xffffffff;
    api()->draw_rectangle(bmap(), &region.region());
    return *this;
  }

  Bitmap &clear() { return clear_rectangle(region()); }

  bool is_empty(const Region &region) const;

  sg_size_t height() const { return m_bmap.area.height; }
  sg_size_t width() const { return m_bmap.area.width; }

  u32 color_count() const;

  Area area() const { return m_bmap.area; }

  Region region() const { return Region(Point(), m_bmap.area); }

  sg_size_t columns() const { return m_bmap.columns; }

  sg_size_t margin_left() const { return m_bmap.margin_top_left.width; }
  sg_size_t margin_right() const { return m_bmap.margin_bottom_right.width; }
  sg_size_t margin_top() const { return m_bmap.margin_top_left.height; }
  sg_size_t margin_bottom() const { return m_bmap.margin_bottom_right.height; }

  Bitmap &set_margin_left(sg_size_t v) {
    m_bmap.margin_top_left.width = v;
    return *this;
  }

  Bitmap &set_margin_right(sg_size_t v) {
    m_bmap.margin_bottom_right.width = v;
    return *this;
  }

  Bitmap &set_margin_top(sg_size_t v) {
    m_bmap.margin_top_left.height = v;
    return *this;
  }

  Bitmap &set_margin_bottom(sg_size_t v) {
    m_bmap.margin_bottom_right.height = v;
    return *this;
  }

  Bitmap &set_margin(sg_size_t v) {
    set_margin_bottom(v);
    set_margin_left(v);
    set_margin_right(v);
    set_margin_top(v);
    return *this;
  }

  Bitmap &set_offset(const Point &point) {
    m_bmap.offset.x = point.x();
    m_bmap.offset.y = point.y();
    return *this;
  }

  var::View to_view();
  const var::View to_view() const;

  const sg_bmap_data_t *bmap_data(const Point &p) const;
  sg_bmap_data_t *bmap_data(const Point &p);

  sg_bmap_t *bmap() { return &m_bmap; }
  const sg_bmap_t *bmap() const { return &m_bmap; }

  operator const sg_bmap_t *() const { return &m_bmap; }

protected:
private:
  friend class BitmapData;

  sg_bmap_t m_bmap = {0};
  sg_color_t calculate_color_sum();
  int set_internal_bits_per_pixel(BitsPerPixel bpp);
  void initialize_members(
    var::View view,
    const Area &area,
    BitsPerPixel bits_per_pixel);
  void calculate_members(const Area &dim);
};

class BitmapData : public Bitmap {
public:
  BitmapData() {}
  BitmapData(const Area &area, BitsPerPixel bits_per_pixel) {
    // calculate size need for new bitmap and allocate the memory
    resize(area, bits_per_pixel);
  }

  BitmapData(const BitmapData &a) {
    m_data = a.m_data;
    resize(a.area(), a.bits_per_pixel());
  }

  BitmapData &operator=(const BitmapData &a) {
    m_data = a.m_data;
    resize(a.area(), a.bits_per_pixel());
    return *this;
  }

  BitmapData(BitmapData &&a) {
    std::swap(m_data, a.m_data);
    resize(a.area(), a.bits_per_pixel());
  }

  BitmapData &operator=(BitmapData &&a) {
    std::swap(m_data, a.m_data);
    resize(a.area(), a.bits_per_pixel());
    return *this;
  }

  BitmapData &resize(const Area &area, BitsPerPixel bits_per_pixel);

  BitmapData &load(const fs::FileObject &file);
  Area load_area(const fs::FileObject &file);

  var::View view() { return var::View(m_data); }
  const var::View view() const { return var::View(m_data); }

private:
  var::Data m_data;
};

} // namespace ux::sgfx

namespace printer {
class Printer;
Printer &operator<<(Printer &printer, const ux::sgfx::Bitmap &a);
Printer &operator<<(Printer &printer, const ux::sgfx::Point &a);
Printer &operator<<(Printer &printer, const ux::sgfx::Region &a);
Printer &operator<<(Printer &printer, const ux::sgfx::Area &a);
Printer &operator<<(Printer &printer, const ux::sgfx::Pen &a);
} // namespace printer

#endif /* UXAPI_UX_SGFX_BITMAP_HPP_ */
