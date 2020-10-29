/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#ifndef UXAPI_UX_DRAW_RICHTEXT_HPP_
#define UXAPI_UX_DRAW_RICHTEXT_HPP_

#include "../Drawing.hpp"
#include "../sgfx/Font.hpp"
#include "../sgfx/IconFont.hpp"

namespace ux::draw {

/*! \brief Text Class
 * \details This is a text label.  The object automatically
 * chooses the correct font height to fit within the area
 * specified.
 *
 * In order for this class to work correctly, sys::Assets::init() must
 * be invoked so that the application is aware of the system fonts.
 *
 */
class RichText : public Drawing, public DrawingComponentProperties<RichText> {
public:
  /*! \details Draw the scaled text as specified by \a attr */
  virtual void draw(const DrawingScaledAttributes &attr);

protected:
  /*! \cond */
  API_ACCESS_COMPOUND(RichText, var::StringView, value);
  API_ACCESS_COMPOUND(RichText, var::StringView, text_font_name);
  API_ACCESS_COMPOUND(RichText, var::StringView, icon_font_name);
  API_ACCESS_FUNDAMENTAL(RichText, const sgfx::Font *, text_font, nullptr);
  API_ACCESS_FUNDAMENTAL(RichText, const sgfx::IconFont *, icon_font, nullptr);
  API_ACCESS_FUNDAMENTAL(RichText, sg_size_t, font_point_size, 0);
  API_ACCESS_FUNDAMENTAL(
    RichText,
    enum sgfx::Font::Style,
    font_style,
    sgfx::Font::Style::regular);
  API_ACCESS_FUNDAMENTAL(RichText, sg_color_t, color, 0);
  bool resolve_fonts(sg_size_t h);

  class RichToken {
  public:
    enum types { type_text, type_icon };

  private:
    API_ACCESS_FUNDAMENTAL(RichToken, sg_size_t, width, 0);
    API_ACCESS_FUNDAMENTAL(RichToken, sg_size_t, height, 0);
    API_ACCESS_FUNDAMENTAL(RichToken, enum types, type, type_text);
    API_ACCESS_FUNDAMENTAL(RichToken, u32, icon_index, 0);
    API_ACCESS_COMPOUND(RichToken, var::String, value);
  };

  /*! \endcond */
};

} // namespace ux::draw

#endif /* UXAPI_UX_DRAW_RICHTEXT_HPP_ */
