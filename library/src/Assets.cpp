// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

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

var::Vector<Assets::FontAsset> Assets::m_font_info_list;
var::Vector<Assets::IconFontAsset> Assets::m_icon_font_info_list;

#if defined NOT_BUILDING
var::Vector<fmt::Svic> Assets::m_vector_path_list;
#endif

bool Assets::m_is_initialized = false;

void Assets::initialize() {
  // search for fonts
  if (m_is_initialized) {
    return;
  }

  var::Array<const char *, 3> asset_directories
    = var::Array(std::array{"/assets", "/home", "/home/assets"});

  for (const auto directory : asset_directories) {
    find_fonts_in_directory(directory);
    find_icons_in_directory(directory);
#if NOT_BUILDING
    find_vector_paths_in_directory(directory);
#endif
  }

  // sort fonts to find a proper match
  m_font_info_list.sort(FontAsset::ascending_style);
  m_font_info_list.sort(FontAsset::ascending_point_size);

  m_icon_font_info_list.sort(IconFontAsset::ascending_point_size);
  m_is_initialized = true;
}

void Assets::clear() {
  for (auto &entry : m_icon_font_info_list) {
    entry.destroy();
  }

  for (auto &entry : m_icon_font_info_list) {
    entry.destroy();
  }
}

void Assets::find_fonts_in_directory(const var::StringView path) {
  API_RETURN_IF_ERROR();
  auto file_list = fs::FileSystem().read_directory(path);
  if (api::ExecutionContext::is_error()) {
    API_RESET_ERROR();
    return;
  }
  size_t entry_count = 0;
  for (const auto &entry : file_list) {
    if (fs::Path::suffix(entry) == "sbf") {
      entry_count++;
    }
  }

  m_font_info_list.reserve(m_font_info_list.count() + entry_count);
  for (const auto &entry : file_list) {
    if (fs::Path::suffix(entry) == "sbf") {
      m_font_info_list.vector().emplace_back(std::move(
        FontAsset().set_info(Font::Info(var::PathString(path) / entry))));
    }
  }
}

void Assets::find_icons_in_directory(const var::StringView path) {
  API_RETURN_IF_ERROR();
  auto file_list = fs::FileSystem().read_directory(path);
  if (api::ExecutionContext::is_error()) {
    API_RESET_ERROR();
    return;
  }
  size_t entry_count = 0;
  for (const auto &entry : file_list) {
    if (fs::Path::suffix(entry) == "sbi") {
      entry_count++;
    }
  }

  m_icon_font_info_list.reserve(m_icon_font_info_list.count() + entry_count);
  for (const auto &entry : file_list) {
    if (fs::Path::suffix(entry) == "sbi") {
      m_icon_font_info_list.vector().emplace_back(std::move(
        IconFontAsset().set_info(Font::Info(var::PathString(path) / entry))));
    }
  }
}

#if defined NOT_BUILDING
void Assets::find_vector_paths_in_directory(const var::StringView path) {
  auto file_list = fs::FileSystem().read_directory(path);

  for (const auto &entry : file_list) {
    if (fs::Path::suffix(entry) == "svic") {
      // format is name-weight-size.sbf
      fmt::Svic svic = fmt::Svic(path / entry);
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

const Assets::IconFontAsset *
Assets::find_icon_font(const FindIconFont &options) {

  initialize();

  u8 closest_point_size = 0;

  // find point size and weight
  for (auto &entry : m_icon_font_info_list) {
    if (options.name() == entry.info().get_name()) {

      if (entry.info().point_size() == options.point_size()) {
        return entry.create();
      }

      if (entry.info().point_size() <= options.point_size()) {
        if (entry.info().point_size() >= closest_point_size) {
          closest_point_size = entry.info().point_size();
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
  for (auto &entry : m_icon_font_info_list) {
    if (options.name() == entry.info().get_name()) {
      if (entry.info().point_size() == closest_point_size) {
        return entry.create();
      }
    }
  }
  return nullptr;
}

const Assets::FontAsset *Assets::find_font(const FindFont &options) {

  initialize();

  u8 closest_point_size = 0;
  Font::Style closest_style = Font::Style::any;

  // find point size and weight
  for (auto &entry : font_info_list()) {
    if (
      ((options.style() == Font::Style::icons)
       && (entry.info().style() == Font::Style::icons))
      || ((options.style() != Font::Style::icons) && (entry.info().style() != Font::Style::icons))) {

      if (
        options.name().is_empty()
        || (options.name() == entry.info().get_name())) {
        if (entry.info().point_size() <= options.point_size()) {
          if (entry.info().point_size() >= closest_point_size) {
            closest_point_size = entry.info().point_size();
            closest_style = entry.info().style();
          }
        }

        if (
          (entry.info().style() == options.style())
          && (entry.info().point_size() == options.point_size())
          && (entry.info().get_name() == options.name() || options.name().is_empty())) {
          return entry.create();
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
  for (auto &entry : font_info_list()) {
    if (
      ((options.style() == Font::Style::icons)
       && (entry.info().style() == Font::Style::icons))
      || ((options.style() != Font::Style::icons) && (entry.info().style() != Font::Style::icons))) {

      if (
        options.name().is_empty()
        || (options.name() == entry.info().get_name())) {

        if (
          (entry.info().point_size() == closest_point_size)
          && (entry.info().style() == closest_style)) {
          return entry.create();
        }
      }
    }
  }

  return nullptr;
}
