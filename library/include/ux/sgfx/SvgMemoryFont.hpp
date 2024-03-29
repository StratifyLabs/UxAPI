/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#ifndef UXAPI_UX_SGFX_SVGMEMORYFONT_HPP_
#define UXAPI_UX_SGFX_SVGMEMORYFONT_HPP_

#include "SvgFont.hpp"

namespace ux::sgfx {

/*! \brief Stratify Vector Graphics Memory Font
 * \details SvgMemoryFont is an implementation of SvgFont
 * where the SVG data is stored in memory (flash or RAM).
 *
 *
 */
class SvgMemoryFont : public SvgFont {
public:
  SvgMemoryFont();
  virtual ~SvgMemoryFont();

protected:
  void draw_char_on_bitmap(
    const sg_font_char_t &ch,
    Bitmap &dest,
    const Point &point) const;
  int load_char(sg_font_char_t &ch, char c, bool ascii) const;
  int load_kerning(u16 first, u16 second) const;
};

} // namespace ux::sgfx

#endif /* UXAPI_UX_SGFX_SVGMEMORYFONT_HPP_ */
