/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#ifndef SAPI_SYS_ASSETS_HPP_
#define SAPI_SYS_ASSETS_HPP_

#include "sgfx/Font.hpp"
#include "sgfx/IconFont.hpp"
#include "sgfx/Vector.hpp"

namespace ux {

/*! \brief Assets Class
 * \details The Assets class is a static class
 * that allows the application to access kernel assets
 * such as fonts, bitmaps, and vector graphics.
 *
 * This class will search the following locations
 * for fonts and graphics files:
 *
 * - /assets
 * - /home
 * - /home/assets
 *
 * Several other classes within the StratifyAPI use
 * this class without the user ever needed to reference it.
 *
 * - draw::Text will lookup fonts using this class
 * - draw::Icon will lookup icons files installed as assets
 *
 *
 */
class Assets {
public:
  /*! \details Initializes system assets.
   *
   * @return Zero
   *
   * This method can be called explicitly, but will
   * be called whenever as needed if not.
   *
   */
  static int initialize();

  /*! \details Returns a read-only reference to the font information list.
   *
   * This list contains a list of the fonts that are available in the system
   * assets.
   *
   */
  static var::Vector<sgfx::Font::Info> &font_info_list() {
    initialize();
    return m_font_info_list;
  }

  class FindFont {
    API_AC(FindFont, var::StringView, name);
    API_AF(FindFont, u16, point_size, 0);
    API_AF(FindFont, sgfx::Font::Style, style, sgfx::Font::Style::regular);
    API_AB(FindFont, exact_match, false);
  };

  static const sgfx::Font::Info *find_font(const FindFont &options);

  static var::Vector<sgfx::IconFont::FontInfo> &icon_font_info_list() {
    initialize();
    return m_icon_font_info_list;
  }

  using FindIconFont = FindFont;
  static const sgfx::IconFont::FontInfo *
  find_icon_font(const FindIconFont &options);

  static void find_fonts_in_directory(const var::StringView path);
  static void find_icons_in_directory(const var::StringView path);
  static void find_vector_paths_in_directory(const var::StringView path);

#if 0
  static const var::Vector<fmt::Svic> &vector_path_list() {
    initialize();
    return m_vector_path_list;
  }

  static sgfx::VectorPath find_vector_path(const var::String &name);
#endif

private:
  static bool m_is_initialized;
  static var::Vector<sgfx::Font::Info> m_font_info_list;
  static var::Vector<sgfx::IconFont::FontInfo> m_icon_font_info_list;
  //  static var::Vector<fmt::Svic> m_vector_path_list;
};

} // namespace ux

#endif /* SAPI_SYS_ASSETS_HPP_ */
