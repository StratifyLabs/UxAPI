/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#ifndef SAPI_SYS_ASSETS_HPP_
#define SAPI_SYS_ASSETS_HPP_

#include "sgfx/Font.hpp"
#include "sgfx/IconFont.hpp"
#include "sgfx/Vector.hpp"

namespace ux {

class Assets {
public:
  static int initialize();

  class FindFont {
    API_AC(FindFont, var::StringView, name);
    API_AF(FindFont, u16, point_size, 0);
    API_AF(FindFont, sgfx::Font::Style, style, sgfx::Font::Style::regular);
    API_AB(FindFont, exact_match, false);
  };

  static const sgfx::Font::Info *find_font(const FindFont &options);

  using FindIconFont = FindFont;
  static const sgfx::IconFont::FontInfo *
  find_icon_font(const FindIconFont &options);

  static void find_fonts_in_directory(const var::StringView path);
  static void find_icons_in_directory(const var::StringView path);
#if 0
  static void find_vector_paths_in_directory(const var::StringView path);

  static const var::Vector<fmt::Svic> &vector_path_list() {
    initialize();
    return m_vector_path_list;
  }

  static sgfx::VectorPath find_vector_path(const var::StringView  name);
#endif

private:
  template <class Font> struct Entry {
    sgfx::Font::Info info;
    fs::File file;
    Font font;

    sgfx::Font::Info *create() {
      if (font.is_valid() == false) {
        file = fs::File(info.file_path());
        font = Font(&file);
      }
      return &info;
    }

    void destroy() {
      if (font.is_valid()) {
        file = fs::File();
        font = Font();
      }
    }

    static bool ascending_point_size(const Entry &a, const Entry &b) {
      return a.info.point_size() < b.info.point_size();
    }

    static bool ascending_style(const Entry &a, const Entry &b) {
      return a.info.style() < b.info.style();
    }
  };

  using FontEntry = Entry<sgfx::Font>;
  using IconFontEntry = Entry<sgfx::IconFont>;

  static bool m_is_initialized;
  static var::Vector<FontEntry> m_font_info_list;
  static var::Vector<IconFontEntry> m_icon_font_info_list;
  //  static var::Vector<fmt::Svic> m_vector_path_list;

  static var::Vector<FontEntry> &font_info_list() {
    initialize();
    return m_font_info_list;
  }

  static var::Vector<IconFontEntry> &icon_font_info_list() {
    initialize();
    return m_icon_font_info_list;
  }
};

} // namespace ux

#endif /* SAPI_SYS_ASSETS_HPP_ */
