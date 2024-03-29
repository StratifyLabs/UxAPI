// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#include "ux/draw/Icon.hpp"
#include "ux/Assets.hpp"

using namespace ux::draw;
using namespace ux::sgfx;

Icon::Icon() {
  m_icon_font = nullptr;
  set_align_center();
  set_align_middle();
}

bool Icon::resolve_font(sg_size_t h) {
  if (this->icon_font() == nullptr) {
    const Assets::IconFontAsset *entry = Assets::find_icon_font(
      Assets::FindIconFont().set_name(m_icon_font_name).set_point_size(h));
    if (entry) {
      this->m_icon_font = &entry->font();
      return true;
    }
    return false;
  }
  return true;
}

void Icon::draw(const DrawingScaledAttributes &attr) {
  sg_point_t top_left;
  int w;
  int h;
  Area d = attr.area();
  sg_point_t p = attr.point();
  const IconFont *f;

  if (resolve_font(d.height()) == false) {
    printf("failed to resolve icon font\n");
    return;
  }

  f = this->icon_font();

  size_t icon_index = f->find(name());
  IconFont::IconInfo icon_info = f->get_info(icon_index);

  h = icon_info.height();
  w = icon_info.width();

  if (is_align_left()) {
    top_left.x = p.x;
  } else if (is_align_right()) {
    top_left.x = p.x + d.width() - w;
  } else {
    top_left.x = p.x + d.width() / 2 - w / 2;
  }

  if (is_align_top()) {
    // top
    top_left.y = p.y;
  } else if (is_align_bottom()) {
    // bottom
    top_left.y = p.y + d.height() - h;
  } else {
    // middle
    top_left.y = p.y + d.height() / 2 - h / 2;
  }

  f->draw(
    icon_index,
    attr.bitmap().set_pen(Pen().set_color(m_color).set_zero_transparent()),
    top_left);
}
