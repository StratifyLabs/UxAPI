// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#ifndef SAPI_SYS_ASSETS_HPP_
#define SAPI_SYS_ASSETS_HPP_

#include "sgfx/Font.hpp"
#include "sgfx/IconFont.hpp"
#include "sgfx/Vector.hpp"

namespace ux {

class Assets {
public:
  template <class Font> class Asset {
    API_AC(Asset<Font>, sgfx::Font::Info, info);
    API_AC(Asset<Font>, Font, font);
    fs::File m_file;

  public:
    const Asset<Font> *create() {
      if (m_font.is_valid() == false) {
        m_file = fs::File(m_info.file_path());
        m_font = Font(&m_file);
      }
      return this;
    }

    void destroy() {
      if (m_font.is_valid()) {
        m_file = fs::File();
        m_font = Font();
      }
    }

    static bool ascending_point_size(const Asset &a, const Asset &b) {
      return a.info().point_size() < b.info().point_size();
    }

    static bool ascending_style(const Asset &a, const Asset &b) {
      return a.info().style() < b.info().style();
    }
  };

  using FontAsset = Asset<sgfx::Font>;
  using IconFontAsset = Asset<sgfx::IconFont>;

  static int initialize();

  class FindFont {
    API_AC(FindFont, var::StringView, name);
    API_AF(FindFont, u16, point_size, 0);
    API_AF(FindFont, sgfx::Font::Style, style, sgfx::Font::Style::regular);
    API_AB(FindFont, exact_match, false);
  };

  static const FontAsset *find_font(const FindFont &options);

  using FindIconFont = FindFont;
  static const IconFontAsset *find_icon_font(const FindIconFont &options);

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
  static bool m_is_initialized;
  static var::Vector<FontAsset> m_font_info_list;
  static var::Vector<IconFontAsset> m_icon_font_info_list;
  //  static var::Vector<fmt::Svic> m_vector_path_list;

  static var::Vector<FontAsset> &font_info_list() {
    initialize();
    return m_font_info_list;
  }

  static var::Vector<IconFontAsset> &icon_font_info_list() {
    initialize();
    return m_icon_font_info_list;
  }
};

} // namespace ux

#endif /* SAPI_SYS_ASSETS_HPP_ */
