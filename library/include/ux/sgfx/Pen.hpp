// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#ifndef UXAPI_UX_SGFX_PEN_HPP_
#define UXAPI_UX_SGFX_PEN_HPP_

#include "Api.hpp"

namespace ux::sgfx {

class PenFlags {
public:
  enum Flags {
    solid /*! Draw solid */ = SG_PEN_FLAG_IS_SOLID,
    invert /*! Draw Invert (XOR) */ = SG_PEN_FLAG_IS_INVERT,
    blend /*! Draw Blend (OR) */ = SG_PEN_FLAG_IS_BLEND,
    erase /*! Draw Erase (NOT AND) */ = SG_PEN_FLAG_IS_ERASE,
    fill /*! Draw Icon with fill points */ = SG_PEN_FLAG_IS_FILL,
    zero_transparent /*! Ignore zero's when copying bitmaps */
    = SG_PEN_FLAG_IS_ZERO_TRANSPARENT
  };
};

API_OR_NAMED_FLAGS_OPERATOR(PenFlags, Flags)

/*! \brief Pen Class
 * \details The pen defines the color, thickness
 * and mode when drawing on bitmaps.
 *
 * Whenever drawing on bitmaps, a pen is associated
 * with the bitmap. The current state of the pen determines
 * how primitives are drawn.
 *
 * First, pens have a color value that is represented
 * by the number of bits linked to the graphics library.
 * For example, when using 1bpp, valid colors values are 0 and 1.
 *
 * Pens can operate in four modes:
 * - Solid: pen color is assigned to bitmap
 * - Blend: pen color is OR'd with bitmap color
 * - Invert: pen color is XOR'd with bitmap color
 * - Erase: pen color is inverted when AND'd with the bitmap color
 *
 * When drawing a source bitmap on a destination bitmap,
 * the pen mode is determined by the destination bitmap's pen.
 * The pen color is ignored and the source bitmap pixel color is used.
 * The source bitmap's pen is not used in bitmap to bitmap drawing operations.
 *
 * Icon vector graphics sometimes include fill points. The destination
 * bitmap's pen determines whether or not the fill operations are performed
 * based on the pen's fill flag (see is_fill() and set_fill()).
 *
 * The pen can also specify a thickness which applies to drawing
 * lines and arcs on bitmaps.
 *
 *
 */
class Pen : public PenFlags {
public:
  Pen();
  Pen(const sg_pen_t &pen) : m_pen(pen) {}

  /*! \details Accesses the pen thickness. */
  u8 thickness() const { return m_pen.thickness; }

  bool is_solid() const {
    return (m_pen.o_flags & SG_PEN_FLAG_NOT_SOLID_MASK) == 0;
  }
  /*! \details Returns true if pen is in invert mode. */
  bool is_invert() const { return (flags() & Flags::invert); }
  /*! \details Returns true if pen is in blend mode. */
  bool is_blend() const { return (flags() & Flags::blend); }
  /*! \details Returns true if pen is in erase mode. */
  bool is_erase() const { return (flags() & Flags::erase); }

  /*! \details Returns true if fill is enabled for pen (used with icons). */
  bool is_fill() const { return (flags() & Flags::fill); }

  /*! \details Access the pen color. */
  sg_color_t color() const { return m_pen.color; }

  /*! \details Sets the pen to a solid color mode (assign value). */
  Pen &set_solid() {
    m_pen.o_flags &= ~SG_PEN_FLAG_NOT_SOLID_MASK;
    return *this;
  }

  /*! \details Sets the pen to inverting color mode (XOR). */
  Pen &set_invert() {
    set_solid();
    enable_flags(Flags::invert);
    return *this;
  }

  /*! \details Sets the pen to a clearing mode (AND). */
  Pen &set_erase() {
    set_solid();
    enable_flags(Flags::erase);
    return *this;
  }

  /*! \details Sets the pen to a blending mode (OR). */
  Pen &set_blend() {
    set_solid();
    enable_flags(Flags::invert);
    return *this;
  }

  /*! \details Causes drawing to ignore zeros when copying bitmaps. */
  Pen &set_zero_transparent() {
    enable_flags(Flags::zero_transparent);
    return *this;
  }

  /*! \details Sets the pen color.
   *
   * @param color The color
   */
  Pen &set_color(sg_color_t color) {
    m_pen.color = color;
    return *this;
  }

  /*! \details Sets the pen thickness. */
  Pen &set_thickness(u8 v) {
    m_pen.thickness = v;
    return *this;
  }

  /*! \details Sets the pen to fill when drawing vector icons. */
  Pen &set_fill(bool v = true) {
    if (v) {
      m_pen.o_flags |= Flags::fill;
    } else {
      m_pen.o_flags &= ~Flags::fill;
    }
    return *this;
  }

  Flags flags() const { return static_cast<Flags>(m_pen.o_flags); }
  Pen &set_flags(Flags flags) {
    m_pen.o_flags = static_cast<u16>(flags);
    return *this;
  }
  const sg_pen_t &pen() const { return m_pen; }
  sg_pen_t &pen() { return m_pen; }
  operator const sg_pen_t &() const { return m_pen; }

private:
  sg_pen_t m_pen;

  void enable_flags(Flags value) { m_pen.o_flags |= static_cast<u16>(value); }
};

} /* namespace ux::sgfx */

#endif /* UXAPI_UX_SGFX_PEN_HPP_ */
