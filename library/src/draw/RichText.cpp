/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#include "ux/draw/RichText.hpp"
#include "ux/Assets.hpp"
#include "var.hpp"

using namespace ux::sgfx;
using namespace ux::draw;

bool RichText::resolve_fonts(sg_size_t h) {
  if (this->text_font() == nullptr) {
    const Assets::FontEntry *entry
      = Assets::find_font(Assets::FindFont()
                            .set_name(text_font_name())
                            .set_point_size(h)
                            .set_style(m_font_style));
    if (entry) {
      this->m_text_font = &entry->font();
    } else {
      printf(
        "didn't find %s %d %d\n",
        text_font_name().get_string().cstring(),
        h,
        (u8)m_font_style);
      return false;
    }
  }

  if (this->icon_font() == nullptr) {
    const Assets::IconFontEntry *entry = Assets::find_icon_font(
      Assets::FindIconFont().set_name(icon_font_name()).set_point_size(h));
    if (entry) {
      this->m_icon_font = &entry->font();
    } else {
      printf(
        "didn't find icon %s %d %d\n",
        text_font_name().get_string().cstring(),
        h,
        (u8)m_font_style);
      return false;
    }
  }
  return true;
}

void RichText::draw(const DrawingScaledAttributes &attr) {
  Area d = attr.area();

  if (resolve_fonts(d.height()) == false) {
    printf("no fonts\n");
    return;
  }

  printf("font %p\n", m_text_font);
  printf("icon font %p\n", m_icon_font);

  // search input for an icon :<icon>:
  // parse the text() -- divide into tokens or either text or icons
  StringViewList raw_token_list = value().split(" ");
  var::Vector<RichToken> rich_token_list;

  rich_token_list.reserve(raw_token_list.count());

  for (const StringView raw_token : raw_token_list) {
    RichToken entry;
    if (
      (raw_token.length() > 2) && (raw_token.front() == ':')
      && (raw_token.back() == ':')) {
      entry.set_type(RichToken::Type::icon)
        .set_value(String(
          raw_token(StringView::GetSubstring().set_position(1).set_length(
            raw_token.length() - 2))));
    } else {
      entry.set_value(String(raw_token));
    }
    rich_token_list.push_back(entry);
  }

  // calculate the total width
  sg_size_t max_height = text_font()->get_height();
  sg_size_t total_width = 0;

  for (RichToken &rich_token : rich_token_list) {
    sg_size_t width;
    sg_size_t height;
    if (rich_token.type() == RichToken::Type::text) {
      width = text_font()->get_width(rich_token.value());
      height = text_font()->get_height();
    } else {
      width = 0;
      rich_token.set_icon_index(icon_font()->find(rich_token.value()));
      IconFont::IconInfo info = icon_font()->get_info(rich_token.icon_index());
      height = info.height();
      width = info.width();
      if (info.height() > max_height) {
        max_height = info.height();
      }
    }
    rich_token.set_height(height);
    rich_token.set_width(width);
    total_width += width;
  }

  sg_point_t p = attr.point();
  Point top_left;
  if (is_align_left()) {
    top_left.set_x(p.x);
  } else if (is_align_right()) {
    top_left.set_x(p.x + d.width() - total_width);
  } else {
    // center by default
    top_left.set_x(p.x + d.width() / 2 - total_width / 2);
  }

  if (is_align_top()) {
    // top
    top_left.set_y(p.y);
  } else if (is_align_bottom()) {
    // bottom
    top_left.set_y(p.y + d.height() - max_height);
  } else {
    // center
    top_left.set_y(p.y + d.height() / 2 - max_height / 2);
  }

  attr.bitmap().set_pen(Pen().set_color(m_color).set_zero_transparent());

  for (const RichToken &rich_token : rich_token_list) {
    sg_size_t y_offset; // center within max height
    y_offset = (max_height - rich_token.height()) / 2;

    if (rich_token.type() == RichToken::Type::icon) {
      icon_font()->draw(
        rich_token.icon_index(),
        attr.bitmap(),
        top_left + Point(0, y_offset));
    } else {
      text_font()->draw(
        rich_token.value(),
        attr.bitmap(),
        top_left + Point(0, y_offset));
    }

    top_left += Point(rich_token.width() + text_font()->space_size(), 0);
  }
}
