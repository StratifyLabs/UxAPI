// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#ifndef UXAPI_UX_DRAWING_HPP_
#define UXAPI_UX_DRAWING_HPP_

#include <sdk/types.h>

#include "sgfx/Bitmap.hpp"
#include "sgfx/Pen.hpp"
#include "sgfx/Theme.hpp"

namespace ux {

/*! \details Drawing size (unsigned) */
typedef u16 drawing_size_t;
/*! \details Drawing position (signed) */
typedef s16 drawing_int_t;

/*! \brief Holds a coordinate (point) in the drawing system */
typedef struct MCU_PACK {
  drawing_int_t x /*! X position */;
  drawing_int_t y /*! Y position */;
} drawing_point_t;

/*! \brief Holds a dimension in the drawing system */
typedef struct MCU_PACK {
  drawing_size_t width /*! Width of the object */;
  drawing_size_t height /*! Height of the object */;
} drawing_area_t;

typedef struct MCU_PACK {
  drawing_point_t point /*! Point in the region */;
  drawing_area_t area /*! Height of the object */;
} drawing_region_t;

sg_color_t color_transparent();

/*! \brief Data used for drawing a draw::Drawing on a bitmap
 * \details This data structure holds a real bitmap but the point
 * and dimensions haven't been mapped to the bitmap.
 * The point \a p and dimension \a d are both in a universal coordinate
 * system from 0 to draw::DrawingAttributes::scale().  For example
 * if draw::DrawingAttributes::scale() is 1000 and p.x and p.y both are equal to
 * 500, the top left corner of any item drawn using these attibutes will be in
 * the center of the target bitmap (regardless of the target bitmap's size).
 */
typedef struct MCU_PACK {
  sgfx::Bitmap *bitmap /*! A pointer to the target bitmap */;
  drawing_region_t
    region /*! The region on the bitmap where draw::Drawing will be drawn */;
} drawing_attr_t;

/*! \brief Attributes for drawing directly on a bitmap using bitmap coordinates
 */
typedef struct MCU_PACK {
  sgfx::Bitmap *bitmap /*! The local bitmap */;
  sg_region_t region /*! The region on the bitmap where draw::DrawingScaled will
                        be drawn */
    ;
  sg_region_t
    window /*! The window on the display where the drawing will be drawn */;
} drawing_scaled_attr_t;

/*! \details This returns a drawing_point_t populated with x and y */
drawing_point_t drawing_point(drawing_int_t x, drawing_int_t y);

/*! \details This returns a drawing_area_t populated with the width and height.
 *
 * @param w Width of the returned data
 * @param h Height of the retured data
 * @return A drawing_area_t with w and h populated as specified
 */
drawing_area_t drawing_area(drawing_size_t w, drawing_size_t h);
drawing_area_t drawing_dim(drawing_size_t w, drawing_size_t h);

class DrawingArea {
public:

  DrawingArea() {
    m_area.height = 0;
    m_area.width = 0;
  }

  DrawingArea(drawing_size_t w, drawing_size_t h) {
    m_area.height = h;
    m_area.width = w;
  }

  DrawingArea(const drawing_area_t &area) { m_area = area; }

  bool is_valid() const { return m_area.width && m_area.height; }

  static DrawingArea maximum();

  drawing_size_t width() const { return m_area.width; }
  drawing_size_t height() const { return m_area.height; }

  DrawingArea &set_width(drawing_size_t value) {
    m_area.width = value;
    return *this;
  }
  DrawingArea &set_height(drawing_size_t value) {
    m_area.height = value;
    return *this;
  }

  const drawing_area_t &area() const { return m_area; }
  drawing_area_t &area() { return m_area; }

  operator const drawing_area_t &() const { return m_area; }

private:
  drawing_area_t m_area;
};

class DrawingPoint {
public:
  DrawingPoint() {
    m_point.x = 0;
    m_point.y = 0;
  }

  DrawingPoint(drawing_int_t x, drawing_int_t y) {
    m_point.x = x;
    m_point.y = y;
  }

  DrawingPoint(const drawing_point_t &point) { m_point = point; }

  static DrawingPoint origin() { return DrawingPoint(0, 0); }

  DrawingPoint operator+(const DrawingPoint &point) const {
    DrawingPoint result(*this);
    result.m_point.x += point.x();
    result.m_point.y += point.y();
    return result;
  }

  DrawingPoint &operator+=(const DrawingPoint &point) {
    *this = *this + point;
    return *this;
  }

  DrawingPoint operator-(const DrawingPoint &point) const {
    DrawingPoint result(*this);
    result.m_point.x -= point.x();
    result.m_point.y -= point.y();
    return result;
  }

  DrawingPoint &operator-=(const DrawingPoint &point) {
    *this = *this - point;
    return *this;
  }

  drawing_int_t x() const { return m_point.x; }
  DrawingPoint &set_x(drawing_int_t value) {
    m_point.x = value;
    return *this;
  }

  drawing_int_t y() const { return m_point.y; }
  DrawingPoint &set_y(drawing_int_t value) {
    m_point.y = value;
    return *this;
  }

  const drawing_point_t &point() const { return m_point; }
  drawing_point_t &point() { return m_point; }

  operator const drawing_point_t &() const { return m_point; }

private:
  drawing_point_t m_point;
};

class DrawingRegion {

public:
  DrawingRegion() { m_region.point = DrawingPoint(0, 0); }

  DrawingRegion(const drawing_region_t &region) { m_region = region; }

  bool is_valid() const { return DrawingArea(m_region.area).is_valid(); }

  DrawingRegion(const DrawingPoint &point, const DrawingArea &area) {
    m_region.point = point;
    m_region.area = area;
  }

  DrawingRegion &operator<<(const DrawingArea &area) {
    m_region.area = area;
    return *this;
  }

  DrawingRegion &operator<<(const DrawingPoint &point) {
    m_region.point = point;
    return *this;
  }

  DrawingArea area() const { return m_region.area; }
  DrawingPoint point() const { return m_region.point; }

  operator const drawing_region_t &() const { return m_region; }

private:
  drawing_region_t m_region;
};

/*! \brief Drawing Attribute Class
 * \details This class contains the information needed to draw various
 * Drawing objects on a bitmap.
 *
 * This class is passed to Drawing::draw() to render graphics on a bitmap.
 *
 * The following code will draw an rectangle in the middle of a 64x64
 * pixel bitmap.
 *
 * ```
 * #include <sgfx.hpp>
 * #include <draw.hpp>
 *
 * Bitmap bitmap(64,64); //64x64 pixel bitmap
 * DrawingAttributes attributes;
 *
 * attributes << bitmap; //apply bitmap to attributes
 *
 * Rectangle().draw(attributes + DrawingPoint(250,250) + DrawingArea(500,500));
 * ```
 *
 * A DrawingAttributes object is always passed to the Drawing::draw() method.
 *
 * The + operator when adding a DrawingPoint will create a new
 * DrawingAttributes object that starts at the specified point. The coordinates
 * are the width of the bitmap / 1000.
 *
 * The + operator when adding a DrawingArea will scale the region of the
 * DrawingAttributes based on the size of the bitmap where 1000 represents
 * full width and height.
 *
 * So in the example above, the top left corner of the rectangle is mapped
 * to 16,16 (64*250/1000=16). The width and height of the rectangle are both
 * 32 (64*500/1000=32).
 *
 * These operators allow the cascading of drawing objects as the region scales.
 * For example, if we create a DrawingAttributes object that is already
 * region limited, the + operators will operate within the existing region.
 *
 * ```
 * Bitmap bitmap(64,64); //64x64 pixel bitmap
 * DrawingAttributes attributes;
 *
 * DrawingRegion region(DrawingPoint(0,0), DrawingArea(500,500));
 * attributes << bitmap << region; //apply bitmap and region to attributes
 *
 * //rectangle will be 500/1000 of 500/1000 of the area of the bitmap
 * Rectangle().draw(attributes + DrawingArea(500,500));
 * ```
 *
 * In this second example, because the DrawingAttributes region was already set
 * to a subregion of the bitmap, the + operator with the DrawingArea scales
 * the region down by 500/1000. So the rectangle is 16,16 (64*500/1000 *
 * 500/1000).
 *
 * Using these operators to cascade is great for creating compound
 * drawing objects like a button.
 *
 * ```
 * class Button: public Drawing {
 * public:
 *  void draw(const DrawingAttributes & attributes){
 *    RoundedRectangle().draw(attributes); //files entire attributes region
 *
 *    //The Icon is scaled down to half the width/height and centered
 *    Icon().set_name("OK").draw(attributes + DrawingPoint(250,250) +
 * DrawingArea(500,500));
 *
 *  }
 * };
 * ```
 *
 */
class DrawingAttributes {
public:
  /*! \details Construct an object */
  DrawingAttributes();

  /*! \details Construct an object from a drawing_attr_t */
  DrawingAttributes(const drawing_attr_t &attr);

  DrawingAttributes(sgfx::Bitmap *bitmap, const DrawingRegion &region) {
    m_attr.bitmap = bitmap;
    m_attr.region = region;
  }

  /*! \details Access the underlying attr object */
  drawing_attr_t &attributes() { return m_attr; }
  const drawing_attr_t &attributes() const { return m_attr; }

  DrawingAttributes(sgfx::Bitmap &bitmap);
  DrawingAttributes(sgfx::Bitmap &bitmap, const DrawingRegion &region);

  bool is_valid() const { return m_attr.bitmap != 0; }

  /*! \details Set the bitmap */
  DrawingAttributes &set_bitmap(sgfx::Bitmap &b) {
    m_attr.bitmap = &b;
    return *this;
  }

  /*! \details Set the dimensions.  Both width and height are from 0 to 1000. */
  DrawingAttributes &set_area(const DrawingArea &value) {
    m_attr.region.area = value;
    return *this;
  }

  /*! \details Set the location.  Both x and y are from 0 to 1000. */
  DrawingAttributes &set_point(const DrawingPoint &p) {
    m_attr.region.point = p;
    return *this;
  }

  /*! \details Return the width */
  drawing_size_t width() const { return m_attr.region.area.width; }
  /*! \details Return the height */
  drawing_size_t height() const { return m_attr.region.area.height; }
  /*! \details Return the x value */
  drawing_int_t x() const { return m_attr.region.point.x; }
  /*! \details Return the y value */
  drawing_int_t y() const { return m_attr.region.point.y; }

  /*! \details Checks to see if bitmap is available. */
  bool is_bitmap_available() const { return m_attr.bitmap != 0; }

  /*! \details Access the bitmap */
  sgfx::Bitmap &bitmap() const { return *(m_attr.bitmap); }

  /*! \details Returns a copy of the region. */
  DrawingRegion region() const { return m_attr.region; }
  /*! \details Returns a copy of the position. */
  DrawingPoint point() const { return m_attr.region.point; }
  /*! \details Returns a copy of the dimensions. */
  DrawingArea area() const { return m_attr.region.area; }

  /*! \details Calculate the scaled height relative to the height of the
   * DrawingAttributes object.
   *
   * @param v The value to calculate
   *
   * This method will calculate a height relative to the heigh of the object
   * and the scale() value.  For example, if the current object maps to a region
   * of a bitmap height of 500 pixels, this method will return a drawing height
   * that will be from 0 to 500 pixels with \a v being from 0 to
   * DrawingAttributes::scale().
   *
   */
  drawing_int_t calculate_height(drawing_int_t v) const;

  /*! \details Calculate the scaled width (width of object on the bitmap) */
  drawing_int_t calculate_width(drawing_int_t v) const;

  /*! \details Add a drawing_point_t offset
   *
   * This operated can be used to create a subregion within the object
   * when used with operation+ (drawing_area_t).
   *
   * For example:
   * \code
   * DrawingAttributes attr0;
   * DrawingAttributes attr1;
   * attr0 = attr1 + DrawingPoint(250,250) + DrawingArea(500,500);
   * \endcode
   *
   * When mapped to the same bitmap, attr0 will be half the height and half
   * the width of attr1. attr0 and attr1 will have the same center point.
   *
   * These operators can be used to draw sub-drawings on drawings. The following
   * example draws a rectangles recursively in an object.
   *
   * ```
   * void draw_rect_recursive(const DrawingAttributes & attr){
   *  static sg_color_t color = 0;
   *  Rectangle().set_color(color++ % 1).draw(attr + DrawingPoint(250,250) +
   *DrawingArea(500,500); if( attr.width() > 100 ){ draw_rect_recursive(attr);
   *	 }
   * }
   * ```
   *
   */
  DrawingAttributes operator+(const drawing_point_t &d) const;
  /*! \details Update the dimension (must come after adding drawing_point_t) */
  DrawingAttributes operator+(const drawing_area_t &d) const;

  DrawingAttributes operator+(const DrawingPoint &d) const;
  DrawingAttributes operator+(const DrawingArea &d) const;

  DrawingAttributes operator+(DrawingRegion region) const {
    return *this + region.point() + region.area();
  }

  /*! \details Calculate dimensions that will map to the bitmap as a square.
   *
   * @param v The maximum width or height
   * @return Square dimensions
   */
  drawing_area_t calculate_square(drawing_size_t v) const;

  /*! \details Calculate dimensions that will map to the bitmap as a square
   * with the given width.
   *
   * @param v The width (height will be calculated)
   * @return Square dimensions
   */
  drawing_area_t calculate_square_width(drawing_size_t v) const;

  /*! \details Calculate dimensions that will map to the bitmap as a square
   * with the given height.
   *
   * @param v The height (width will be calculated)
   * @return Square dimensions
   */
  drawing_area_t calculate_square_height(drawing_size_t v) const;

  static sg_area_t calculate_area_on_bitmap(const DrawingAttributes &attr);
  static sg_size_t calculate_height_on_bitmap(const DrawingAttributes &attr);
  static sg_size_t calculate_height_on_bitmap(
    const DrawingAttributes &attr,
    drawing_size_t value);
  static sg_size_t calculate_width_on_bitmap(const DrawingAttributes &attr);
  static sg_size_t calculate_width_on_bitmap(
    const DrawingAttributes &attr,
    drawing_size_t value);
  static sg_point_t calculate_point_on_bitmap(const DrawingAttributes &attr);
  static sg_region_t calculate_region_on_bitmap(const DrawingAttributes &attr);

  sg_area_t calculate_area_on_bitmap() const {
    return calculate_area_on_bitmap(*this);
  }
  sg_size_t calculate_height_on_bitmap() const {
    return calculate_height_on_bitmap(*this);
  }
  sg_size_t calculate_height_on_bitmap(drawing_size_t value) const {
    return calculate_height_on_bitmap(*this, value);
  }
  sg_size_t calculate_width_on_bitmap() const {
    return calculate_width_on_bitmap(*this);
  }
  sg_size_t calculate_width_on_bitmap(drawing_size_t value) const {
    return calculate_width_on_bitmap(*this, value);
  }
  sg_point_t calculate_point_on_bitmap() const {
    return calculate_point_on_bitmap(*this);
  }
  sg_region_t calculate_region_on_bitmap() const {
    return calculate_region_on_bitmap(*this);
  }

  drawing_size_t calculate_height_on_drawing(sg_size_t height);
  drawing_size_t calculate_width_on_drawing(sg_size_t width);

  /*! \details This value determines how all objects are scaled.
   *
   * The default value is 1000. This means a value of 500 is half the target
   * bitmap.
   *
   * sg_size_t * drawing_size_t / scale() = sg_size_t (drawing_size_t cancels
   * with scale())
   *
   */
  static drawing_size_t scale() { return 1000; }

private:
  /*! \cond */
  drawing_attr_t m_attr;
  /*! \endcond */
};

/*! \brief Scaled Drawing Attributes
 * \details This is similar to draw::DrawingAttributes but the point
 * and area have been scaled to fit in the target bitmap.
 */
class DrawingScaledAttributes {
public:
  DrawingScaledAttributes() {}
  DrawingScaledAttributes(const DrawingAttributes &attr) {
    set(
      attr.bitmap(),
      attr.calculate_point_on_bitmap(),
      attr.calculate_area_on_bitmap());
  }

  operator drawing_scaled_attr_t() { return m_attr; }

  void set(sgfx::Bitmap &b, sg_point_t p, sg_area_t d);

  /*! \details Assign a value to the bitmap pointer using a reference. */
  DrawingScaledAttributes &set_bitmap(sgfx::Bitmap &b) {
    m_attr.bitmap = &b;
    return *this;
  }
  /*! \details Assigns area. */
  DrawingScaledAttributes &set_area(const sgfx::Area &area) {
    m_attr.region.area = area.area();
    return *this;
  }
  /*! \details Sets the height of the object */
  DrawingScaledAttributes &set_height(sg_size_t h) {
    m_attr.region.area.height = h;
    return *this;
  }
  /*! \details Sets the width of the object. */
  DrawingScaledAttributes &set_width(sg_size_t w) {
    m_attr.region.area.height = w;
    return *this;
  }
  /*! \details Sets the x value of the object. */
  DrawingScaledAttributes &set_x(sg_int_t x) {
    m_attr.region.point.x = x;
    return *this;
  }
  /*! \details Sets the y value of the object. */
  DrawingScaledAttributes &set_y(sg_int_t y) {
    m_attr.region.point.y = y;
    return *this;
  }

  /*! \details Assign the position */
  void set_point(sg_point_t p) { m_attr.region.point = p; }

  sgfx::Bitmap &bitmap() const { return *m_attr.bitmap; }

  sgfx::Region region() const { return m_attr.region; }
  sgfx::Point point() const { return m_attr.region.point; }
  sgfx::Area area() const { return m_attr.region.area; }
  drawing_scaled_attr_t &attributes() { return m_attr; }
  const drawing_scaled_attr_t &attributes() const { return m_attr; }

  /*! \details Return the width */
  sg_size_t width() const { return m_attr.region.area.width; }
  /*! \details Return the height */
  sg_size_t height() const { return m_attr.region.area.height; }
  /*! \details Return the x value */
  sg_int_t x() const { return m_attr.region.point.x; }
  /*! \details Return the y value */
  sg_int_t y() const { return m_attr.region.point.y; }

  /*! \details Add an sg_point_t */
  DrawingScaledAttributes operator+(const sgfx::Point &p) const;
  /*! \details Add an sg_area_t */
  DrawingScaledAttributes operator+(const sgfx::Area &a) const;

  /*! \details Calculate a width value
   *
   * @param v Unscaled drawing dimensions
   * @return
   */
  sg_size_t calculate_width(drawing_size_t v) const;
  sg_size_t calculate_height(drawing_size_t v) const;

private:
  drawing_scaled_attr_t m_attr;
};

template <typename T> class DrawingComponentProperties {
public:
  bool is_align_left() const { return m_flags & flag_align_left; }
  bool is_align_right() const { return m_flags & (flag_align_right); }
  bool is_align_center() const {
    return (m_flags & (flag_align_left | flag_align_right)) == 0;
  }
  bool is_align_top() const { return m_flags & (flag_align_top); }
  bool is_align_bottom() const { return m_flags & (flag_align_bottom); }
  bool is_align_middle() const {
    return (m_flags & (flag_align_top | flag_align_bottom)) == 0;
  }

  T & set_font_name(const var::StringView value){
    m_font_name = value;
    return derived_this();
  }

  var::StringView font_name() const {
    return m_font_name;
  }

  T &set_align_left(bool v = true) {
    v ? m_flags |= flag_align_left : 0;
    return derived_this();
  }
  T &set_align_right(bool v = true) {
    v ? m_flags |= flag_align_right : 0;
    return derived_this();
  }
  T &set_align_center(bool v = true) {
    if (v) {
      m_flags &= ~(flag_align_left | flag_align_right);
    }
    return derived_this();
  }

  T &set_align_top(bool v = true) {
    v ? m_flags |= flag_align_top : 0;
    return derived_this();
  }
  T &set_align_bottom(bool v = true) {
    v ? m_flags |= flag_align_bottom : 0;
    return derived_this();
  }
  T &set_align_middle(bool v = true) {
    if (v) {
      m_flags &= ~(flag_align_top | flag_align_bottom);
    }
    return derived_this();
  }

  u32 alignment() const { return m_flags; }

  T &set_alignment(u32 value) {
    m_flags = value;
    return derived_this();
  }

  T &set_border(u8 value) {
    m_border.top = value;
    m_border.bottom = value;
    m_border.left = value;
    m_border.right = value;
    return derived_this();
  }

  T &set_border_size(u8 value) { return set_border(value); }

  T &set_top_border(u8 value) {
    m_border.top = value;
    return derived_this();
  }

  T &set_bottom_border(u8 value) {
    m_border.bottom = value;
    return derived_this();
  }

  T &set_left_border(u8 value) {
    m_border.left = value;
    return derived_this();
  }

  T &set_right_border(u8 value) {
    m_border.right = value;
    return derived_this();
  }

  T &set_horizontal_border(u8 value) {
    m_border.right = value;
    m_border.left = value;
    return derived_this();
  }

  T &set_vertical_border(u8 value) {
    m_border.top = value;
    m_border.bottom = value;
    return derived_this();
  }

  T &set_padding(u8 value) {
    m_padding.top = value;
    m_padding.bottom = value;
    m_padding.left = value;
    m_padding.right = value;
    return derived_this();
  }

  T &set_vertical_padding(u8 value) {
    m_padding.top = value;
    m_padding.bottom = value;
    return derived_this();
  }

  T &set_top_padding(u8 value) {
    m_padding.top = value;
    return derived_this();
  }

  T &set_bottom_padding(u8 value) {
    m_padding.bottom = value;
    return derived_this();
  }

  T &set_horizontal_padding(u8 value) {
    m_padding.left = value;
    m_padding.right = value;
    return derived_this();
  }

  T &set_left_padding(u8 value) {
    m_padding.left = value;
    return derived_this();
  }

  T &set_right_padding(u8 value) {
    m_padding.right = value;
    return derived_this();
  }

  T &set_margin(u8 value) {
    m_margin.top = value;
    m_margin.bottom = value;
    m_margin.left = value;
    m_margin.right = value;
    return derived_this();
  }

  T &set_vertical_margin(u8 value) {
    m_margin.top = value;
    m_margin.bottom = value;
    return derived_this();
  }

  T &set_top_margin(u8 value) {
    m_margin.top = value;
    return derived_this();
  }

  T &set_bottom_margin(u8 value) {
    m_margin.bottom = value;
    return derived_this();
  }

  T &set_horizontal_margin(u8 value) {
    m_margin.left = value;
    m_margin.right = value;
    return derived_this();
  }

  T &set_left_margin(u8 value) {
    m_margin.left = value;
    return derived_this();
  }

  T &set_right_margin(u8 value) {
    m_margin.right = value;
    return derived_this();
  }

  T &set_outline(u8 value) {
    m_margin.top = value;
    m_margin.bottom = value;
    m_margin.left = value;
    m_margin.right = value;
    return derived_this();
  }

  T &set_vertical_outline(u8 value) {
    m_outline.top = value;
    m_outline.bottom = value;
    return derived_this();
  }

  T &set_top_outline(u8 value) {
    m_outline.top = value;
    return derived_this();
  }

  T &set_bottom_outline(u8 value) {
    m_outline.bottom = value;
    return derived_this();
  }

  T &set_horizontal_outline(u8 value) {
    m_outline.left = value;
    m_outline.right = value;
    return derived_this();
  }

  T &set_left_outline(u8 value) {
    m_outline.left = value;
    return derived_this();
  }

  T &set_right_outline(u8 value) {
    m_outline.right = value;
    return derived_this();
  }

  u8 border_size() const {
    return (m_border.top + m_border.bottom + m_border.left + m_border.right)
           / 4;
  }

  u8 left_border() const { return m_border.left; }

  u8 right_border() const { return m_border.right; }

  u8 top_border() const { return m_border.top; }

  u8 bottom_border() const { return m_border.bottom; }

  u8 vertical_padding() const { return m_padding.top + m_padding.bottom; }

  u8 horizontal_padding() const { return m_padding.left + m_padding.right; }

  u8 left_padding() const { return m_padding.left; }

  u8 right_padding() const { return m_padding.right; }

  u8 top_padding() const { return m_padding.top; }

  u8 bottom_padding() const { return m_padding.bottom; }

  u8 vertical_margin() const { return m_margin.top + m_margin.bottom; }

  u8 horizontal_margin() const { return m_margin.left + m_margin.right; }

  u8 left_margin() const { return m_margin.left; }

  u8 right_margin() const { return m_margin.right; }

  u8 top_margin() const { return m_margin.top; }

  u8 bottom_margin() const { return m_margin.bottom; }

  u8 vertical_outline() const { return m_outline.top + m_outline.bottom; }

  u8 horizontal_outline() const { return m_outline.left + m_outline.right; }

  u8 left_outline() const { return m_outline.left; }

  u8 right_outline() const { return m_outline.right; }

  u8 top_outline() const { return m_outline.top; }

  u8 bottom_outline() const { return m_outline.bottom; }

protected:
  enum flags {
    flag_align_left = 1 << 0,
    flag_align_right = 1 << 1,
    flag_align_top = 1 << 2,
    flag_align_bottom = 1 << 3
  };

  typedef struct {
    u8 top;
    u8 right;
    u8 bottom;
    u8 left;
  } dimension_t;

  sg_size_t calculate_border_size(sg_size_t height) {
    return calculate_pixel_size(border_size(), height);
  }

  sgfx::Region calculate_region_inside(
    const sgfx::Region &region,
    const dimension_t &dimension) const {

    dimension_t effective;
    effective.left = calculate_pixel_size(dimension.left, region.width());

    effective.right = calculate_pixel_size(dimension.right, region.width());

    effective.top = calculate_pixel_size(dimension.top, region.height());

    effective.bottom = calculate_pixel_size(dimension.bottom, region.height());

    return sgfx::Region(
      sgfx::Point(region.x() + effective.left, region.y() + effective.top),
      sgfx::Area(
        region.width() - (effective.left + effective.right),
        region.height() - (effective.top + effective.bottom)));
  }

  sgfx::Region
  calculate_region_inside_outline(const sgfx::Region &region) const {
    return calculate_region_inside(region, m_outline);
  }

  sgfx::Region
  calculate_region_inside_margin(const sgfx::Region &region) const {
    return calculate_region_inside(region, m_margin);
  }

  sgfx::Region
  calculate_region_inside_border(const sgfx::Region &region) const {
    return calculate_region_inside(
      calculate_region_inside_margin(region),
      m_border);
  }

  sgfx::Region
  calculate_region_inside_padding(const sgfx::Region &region) const {
    return calculate_region_inside(
      calculate_region_inside_border(region),
      m_padding);
  }

  void draw_base_properties(
    sgfx::Bitmap &bitmap,
    const sgfx::Region &region,
    const sgfx::Theme *theme) const {

    const sgfx::Region inside_outline = calculate_region_inside_outline(region);
    const sgfx::Region inside_margin = calculate_region_inside_margin(region);

    bitmap.set_pen(sgfx::Pen().set_color(theme->border_color()))
      .draw_rectangle(region)
      .set_pen(sgfx::Pen().set_color(theme->background_color()))
      .draw_rectangle(inside_outline)
      .set_pen(sgfx::Pen().set_color(theme->border_color()))
      .draw_rectangle(inside_margin)
      .set_pen(sgfx::Pen().set_color(theme->color()))
      .draw_rectangle(calculate_region_inside_border(region));
  }

private:
  dimension_t m_padding = {5, 5, 5, 5};
  dimension_t m_margin = {0};
  dimension_t m_border = {1, 1, 1, 1};
  dimension_t m_outline = {0};
  var::StringView m_font_name;

  u32 m_flags = 0; // default is middle/center

  static sg_size_t calculate_pixel_size(u8 value, sg_size_t dimension) {
    sg_size_t result = (dimension * value + 100) / 200;
    if (!result && value) {
      result = 1;
    }
    return result;
  }

  T &derived_this() { return static_cast<T &>(*this); }
};

#define DRAWING_COMPONENT_PROPERTIES_ACCESS(DERIVED_TYPE)                      \
  DERIVED_TYPE &set_align_left(bool v = true) {                                \
    DrawingComponentProperties::set_align_left(v);                             \
    return *this;                                                              \
  }                                                                            \
  DERIVED_TYPE &set_align_right(bool v = true) {                               \
    DrawingComponentProperties::set_align_right(v);                            \
    return *this;                                                              \
  }                                                                            \
  DERIVED_TYPE &set_align_center(bool v = true) {                              \
    DrawingComponentProperties::set_align_center(v);                           \
    return *this;                                                              \
  }                                                                            \
  DERIVED_TYPE &set_align_top(bool v = true) {                                 \
    DrawingComponentProperties::set_align_top(v);                              \
    return *this;                                                              \
  }                                                                            \
  DERIVED_TYPE &set_align_bottom(bool v = true) {                              \
    DrawingComponentProperties::set_align_bottom(v);                           \
    return *this;                                                              \
  }                                                                            \
  DERIVED_TYPE &set_align_middle(bool v = true) {                              \
    DrawingComponentProperties::set_align_middle(v);                           \
    return *this;                                                              \
  }                                                                            \
  DERIVED_TYPE &set_alignment(u32 value) {                                     \
    DrawingComponentProperties::set_alignment(value);                          \
    return *this;                                                              \
  }                                                                            \
  DERIVED_TYPE &set_border(u8 value) {                                         \
    DrawingComponentProperties::set_border(value);                             \
    return *this;                                                              \
  }                                                                            \
  DERIVED_TYPE &set_top_border(u8 value) {                                     \
    DrawingComponentProperties::set_top_border(value);                         \
    return *this;                                                              \
  }                                                                            \
  DERIVED_TYPE &set_bottom_border(u8 value) {                                  \
    DrawingComponentProperties::set_bottom_border(value);                      \
    return *this;                                                              \
  }                                                                            \
  DERIVED_TYPE &set_left_border(u8 value) {                                    \
    DrawingComponentProperties::set_left_border(value);                        \
    return *this;                                                              \
  }                                                                            \
  DERIVED_TYPE &set_right_border(u8 value) {                                   \
    DrawingComponentProperties::set_right_border(value);                       \
    return *this;                                                              \
  }                                                                            \
  DERIVED_TYPE &set_horizontal_border(u8 value) {                              \
    DrawingComponentProperties::set_horizontal_border(value);                  \
    return *this;                                                              \
  }                                                                            \
  DERIVED_TYPE &set_vertical_border(u8 value) {                                \
    DrawingComponentProperties::set_vertical_border(value);                    \
    return *this;                                                              \
  }                                                                            \
  DERIVED_TYPE &set_padding(u8 value) {                                        \
    DrawingComponentProperties::set_padding(value);                            \
    return *this;                                                              \
  }                                                                            \
  DERIVED_TYPE &set_top_padding(u8 value) {                                    \
    DrawingComponentProperties::set_top_padding(value);                        \
    return *this;                                                              \
  }                                                                            \
  DERIVED_TYPE &set_bottom_padding(u8 value) {                                 \
    DrawingComponentProperties::set_bottom_padding(value);                     \
    return *this;                                                              \
  }                                                                            \
  DERIVED_TYPE &set_left_padding(u8 value) {                                   \
    DrawingComponentProperties::set_left_padding(value);                       \
    return *this;                                                              \
  }                                                                            \
  DERIVED_TYPE &set_right_padding(u8 value) {                                  \
    DrawingComponentProperties::set_right_padding(value);                      \
    return *this;                                                              \
  }                                                                            \
  DERIVED_TYPE &set_horizontal_padding(u8 value) {                             \
    DrawingComponentProperties::set_horizontal_padding(value);                 \
    return *this;                                                              \
  }                                                                            \
  DERIVED_TYPE &set_vertical_padding(u8 value) {                               \
    DrawingComponentProperties::set_vertical_padding(value);                   \
    return *this;                                                              \
  }                                                                            \
  DERIVED_TYPE &set_margin(u8 value) {                                         \
    DrawingComponentProperties::set_margin(value);                             \
    return *this;                                                              \
  }                                                                            \
  DERIVED_TYPE &set_top_margin(u8 value) {                                     \
    DrawingComponentProperties::set_top_margin(value);                         \
    return *this;                                                              \
  }                                                                            \
  DERIVED_TYPE &set_bottom_margin(u8 value) {                                  \
    DrawingComponentProperties::set_bottom_margin(value);                      \
    return *this;                                                              \
  }                                                                            \
  DERIVED_TYPE &set_left_margin(u8 value) {                                    \
    DrawingComponentProperties::set_left_margin(value);                        \
    return *this;                                                              \
  }                                                                            \
  DERIVED_TYPE &set_right_margin(u8 value) {                                   \
    DrawingComponentProperties::set_right_margin(value);                       \
    return *this;                                                              \
  }                                                                            \
  DERIVED_TYPE &set_horizontal_margin(u8 value) {                              \
    DrawingComponentProperties::set_horizontal_margin(value);                  \
    return *this;                                                              \
  }                                                                            \
  DERIVED_TYPE &set_vertical_margin(u8 value) {                                \
    DrawingComponentProperties::set_vertical_margin(value);                    \
    return *this;                                                              \
  }                                                                            \
  DERIVED_TYPE &set_outline(u8 value) {                                        \
    DrawingComponentProperties::set_outline(value);                            \
    return *this;                                                              \
  }                                                                            \
  DERIVED_TYPE &set_top_outline(u8 value) {                                    \
    DrawingComponentProperties::set_top_outline(value);                        \
    return *this;                                                              \
  }                                                                            \
  DERIVED_TYPE &set_bottom_outline(u8 value) {                                 \
    DrawingComponentProperties::set_bottom_outline(value);                     \
    return *this;                                                              \
  }                                                                            \
  DERIVED_TYPE &set_left_outline(u8 value) {                                   \
    DrawingComponentProperties::set_left_outline(value);                       \
    return *this;                                                              \
  }                                                                            \
  DERIVED_TYPE &set_right_outline(u8 value) {                                  \
    DrawingComponentProperties::set_right_outline(value);                      \
    return *this;                                                              \
  }                                                                            \
  DERIVED_TYPE &set_horizontal_outline(u8 value) {                             \
    DrawingComponentProperties::set_horizontal_outline(value);                 \
    return *this;                                                              \
  }                                                                            \
  DERIVED_TYPE &set_vertical_outline(u8 value) {                               \
    DrawingComponentProperties::set_vertical_outline(value);                   \
    return *this;                                                              \
  }

class Drawing : public api::ExecutionContext {
public:
  Drawing();

  static sg_size_t width(sg_size_t scale, sg_area_t d);
  static sg_size_t height(sg_size_t scale, sg_area_t d);

  /*! \details This method draws the object using the specified drawing
   * attributes.
   *
   * The attributes specify which bitmap to draw on, what size to draw, and
   * where to draw. The dimensions and position are scaled to fit on the bitmap.
   *
   */
  virtual void draw(const DrawingAttributes &attr);

  void draw(
    const DrawingAttributes &attr,
    const DrawingPoint &point,
    const DrawingArea &area) {
    draw(attr + point + area);
  }

  /*! \details This method will clear the pixels in the area of the bitmap
   * specified.
   *
   * @param attr Specifies the bitmap and area
   * @param v Specifies the fill pattern
   */
  static void clear(const DrawingAttributes &attr);

  /*! \brief Sets the scale value (see Element::scale() for details). */
  static void set_scale(drawing_size_t s) { m_scale = s; }

  /*! \details This methods draws the drawing on the specified attributes.
   *
   * @param attr Specifies the bitmap, point and area to draw the drawing
   */
  virtual void draw(const DrawingScaledAttributes &attr);

  static drawing_size_t scale() { return m_scale; }

protected:
  /*! \cond */

  sg_point_t point_on_bitmap(
    sgfx::Bitmap &b,
    drawing_size_t x,
    drawing_size_t y,
    sg_area_t d);
  sg_area_t dim_on_bitmap(sgfx::Bitmap &b) const;

private:
  static drawing_size_t m_scale;

  static void
  draw_rectangle(const DrawingAttributes &attr, const sgfx::Pen &pen);
  /*! \endcond */
};

} // namespace ux

#endif /* SAPI_DRAW_DRAWING_HPP_ */
