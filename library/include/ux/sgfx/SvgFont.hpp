/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#ifndef UXAPI_UX_SGFX_SVGFONT_HPP_
#define UXAPI_UX_SGFX_SVGFONT_HPP_

#include "Font.hpp"

namespace ux::sgfx {

/*! \brief Stratify Vector Graphics Font
 * \details The SvgFont class is used to draw
 * fonts using vector graphic data rather then
 * bitmap data.
 *
 */
class SvgFont : public Font {
public:
  SvgFont();
  virtual ~SvgFont();

  virtual sg_size_t get_height() const { return m_height; }
  virtual sg_size_t get_width() const { return m_width; }

  void set_height(sg_size_t height);

private:
  sg_size_t m_height;
  sg_size_t m_width;
};

} // namespace ux::sgfx

#endif /* UXAPI_UX_SGFX_SVGFONT_HPP_ */
