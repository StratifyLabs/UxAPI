/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
// Copyright 2011-2017 Tyler Gilbert; All Rights Reserved

#include <limits.h>

#include <fs/Dir.hpp>
#include <fs/FileSystem.hpp>
#include <fs/Path.hpp>
#include <var/Tokenizer.hpp>

#include "ux/Assets.hpp"

//#include "sys/requests.h"
#include "ux/sgfx/Font.hpp"
#include "ux/sgfx/IconFont.hpp"

using namespace ux;
using namespace ux::sgfx;

var::Vector<Font::Info> Assets::m_font_info_list;
var::Vector<IconFont::FontInfo> Assets::m_icon_font_info_list;

#if defined NOT_BUILDING
var::Vector<fmt::Svic> Assets::m_vector_path_list;
#endif

bool Assets::m_is_initialized = false;

int Assets::initialize() {
  // search for fonts
  if (m_is_initialized) {
    return 0;
  }

  var::Vector<const char *> asset_directories
    = {"/assets", "/home", "/home/assets"};

  for (const auto directory : asset_directories) {
    find_fonts_in_directory(directory);
    find_icons_in_directory(directory);
#if NOT_BUILDING
    find_vector_paths_in_directory(directory);
#endif
  }

  // sort fonts to find a proper match
  m_font_info_list.sort(Font::Info::ascending_style);
  m_font_info_list.sort(Font::Info::ascending_point_size);

  m_is_initialized = true;
  return 0;
}

void Assets::find_fonts_in_directory(const var::StringView path) {
  auto file_list = fs::FileSystem().read_directory(path);
  for (const auto &entry : file_list) {
    if (fs::Path::suffix(entry) == "sbf") {
      m_font_info_list.push_back(Font::Info(path + "/" + entry));
    }
  }
}

void Assets::find_icons_in_directory(const var::StringView path) {
  auto file_list = fs::FileSystem().read_directory(path);
  for (const auto &entry : file_list) {
    if (fs::Path::suffix(entry) == "sbi") {
      m_icon_font_info_list.push_back(IconFont::FontInfo(path + "/" + entry));
    }
  }
}

#if defined NOT_BUILDING
void Assets::find_vector_paths_in_directory(const var::StringView path) {
  auto file_list = fs::FileSystem().read_directory(path);

  for (const auto &entry : file_list) {
    if (fs::Path::suffix(entry) == "svic") {
      // format is name-weight-size.sbf
      fmt::Svic svic = fmt::Svic(path + "/" + entry);
      svic.set_keep_open();
      m_vector_path_list.push_back(svic);
    }
  }
}
#endif

#if defined NOT_BUILDING
sgfx::VectorPath Assets::find_vector_path(const var::StringView name) {
  initialize();
  for (u32 i = 0; i < m_vector_path_list.count(); i++) {
    for (u32 j = 0; j < m_vector_path_list.at(i).count(); j++) {
      if (m_vector_path_list.at(i).name_at(j) == name) {
        return m_vector_path_list.at(i).at(j);
      }
    }
  }
  return sgfx::VectorPath();
}
#endif

const sgfx::IconFont::FontInfo *
Assets::find_icon_font(const FindIconFont &options) {

  initialize();

  u32 active_icon_fonts = 0;
  for (const auto &icon_font_info : m_icon_font_info_list) {
    if (icon_font_info.is_valid()) {
      active_icon_fonts++;
    }
  }

  if (active_icon_fonts > 2) {
    for (auto &icon_font_info : m_icon_font_info_list) {
      if (icon_font_info.is_valid()) {
#if defined NOT_BUILDING
        icon_font_info.destroy_icon_font();
#endif
      }
    }
  }

  u8 closest_point_size = 0;

  // find point size and weight
  for (auto &info : m_icon_font_info_list) {
    if (options.name() == info.name()) {

      if (info.point_size() == options.point_size()) {
        // exact match
        // if (info.icon_font() == nullptr) {
        //  info.icon_font_file().open(info.path(), fs::OpenFlags::read_only());
        //  info.set_icon_font(new IconFont(info.icon_font_file()));
        //}
        return &info;
      }

      if (info.point_size() <= options.point_size()) {
        if (info.point_size() >= closest_point_size) {
          closest_point_size = info.point_size();
        }
      }
    }
  }

  // could not find an exact match
  if (options.is_exact_match()) {
    return nullptr;
  }

  // first pass is to find the exact style in a point size that is less than or
  // equal
  for (auto &info : m_icon_font_info_list) {
    if (options.name() == info.name()) {
      if (info.point_size() == closest_point_size) {
        // if (info.icon_font() == nullptr) {
        // info.create_icon_font();
        //}
        return &info;
      }
    }
  }
  return nullptr;
}

const sgfx::Font::Info *Assets::find_font(const FindFont &options) {

  initialize();

  u32 active_fonts = 0;
  for (const auto &font_info : m_font_info_list) {
    if (font_info.is_valid()) {
      active_fonts++;
    }
  }

  if (active_fonts > 1) {
    for (auto &font_info : m_font_info_list) {
      if (font_info.is_valid()) {
        // font_info.destroy_font();
      }
    }
  }

  u8 closest_point_size = 0;
  Font::Style closest_style = Font::Style::any;

  // find point size and weight
  for (const auto &info : font_info_list()) {

    if (
      ((options.style() == Font::Style::icons)
       && (info.style() == Font::Style::icons))
      || ((options.style() != Font::Style::icons) && (info.style() != Font::Style::icons))) {

      if (options.name().is_empty() || (options.name() == info.name())) {
        if (info.point_size() <= options.point_size()) {
          if (info.point_size() >= closest_point_size) {
            closest_point_size = info.point_size();
            closest_style = info.style();
          }
        }

        if (
          (info.style() == options.style())
          && (info.point_size() == options.point_size())
          && (info.name() == options.name() || options.name().is_empty())) {
          // exact match
          // if (info.font() == nullptr) {
          //  info.create_font();
          //  if (info.is_valid()) {
          //    info.font()->set_space_size(info.font()->get_height() / 4);
          //  }
          // }
          return &info;
        }
      }
    }
  }

  // could not find an exact match
  if (options.is_exact_match()) {
    return nullptr;
  }

  // first pass is to find the exact style in a point size that is less than or
  // equal
  for (const auto &info : font_info_list()) {

    if (
      ((options.style() == Font::Style::icons)
       && (info.style() == Font::Style::icons))
      || ((options.style() != Font::Style::icons) && (info.style() != Font::Style::icons))) {

      if (options.name().is_empty() || (options.name() == info.name())) {

        if (
          (info.point_size() == closest_point_size)
          && (info.style() == closest_style)) {
          // if (info.font() == nullptr) {
          //  info.create_font();
          //  if (info.is_valid()) {
          //    info.font()->set_space_size(info.font()->get_height() / 4);
          //  }
          //}
          return &info;
        }
      }
    }
  }

  return nullptr;
}
