#include <var.hpp>

#include "ux/sgfx/IconFont.hpp"

using namespace ux::sgfx;

#if defined NOT_BUILDING
IconFont::FontInfo::FontInfo(const var::StringView path) {
  m_path = fs::Path(path);

  StringViewList tokens = fs::Path(path).name().split("-.");

  if (tokens.count() != 3) {
    m_point_size = 0;
  } else {
    m_icon_font = nullptr;
    m_name = tokens.at(0);
    m_point_size = tokens.at(1).to_unsigned_long();
    // m_file = std::move(fs::File(path()));
    m_icon_font = new IconFont(m_file);
  }
}
#endif

IconFont::IconFont(const fs::File &file) : m_file(file) { refresh(); }

size_t IconFont::find(const var::StringView name) const {
  for (size_t i = 0; i < m_list.count(); i++) {
    if (this->get_info(i).name() == name) {
      return i;
    }
  }
  return m_list.count();
}

IconFont &IconFont::refresh() {
  m_list.clear();

  m_file.seek(0).read(var::View(m_header));

  for (u32 i = 0; i < m_header.icon_count; i++) {
    sg_font_icon_t icon;
    m_file.read(View(icon));
    if (is_error()) {
      m_list.clear();
    }

    m_list.push_back(icon);
  }

  m_master_canvas_idx = -1;
  m_master_canvas.resize(
    Area(m_header.canvas_width, m_header.canvas_height),
    static_cast<Bitmap::BitsPerPixel>(m_header.bits_per_pixel));

  return *this;
}

const IconFont &
IconFont::draw(size_t offset, Bitmap &dest, const Point &point) const {

  API_ASSERT(offset < m_list.count());

  const sg_font_icon_t &icon = m_list.at(offset);

  if (icon.canvas_idx != m_master_canvas_idx) {
    const u32 file_offset
      = m_header.size + icon.canvas_idx * m_master_canvas.view().size();

    m_file.seek(file_offset).read(m_master_canvas.view());
    m_master_canvas_idx = icon.canvas_idx;
  }

  dest.draw_sub_bitmap(
    point,
    Bitmap(m_master_canvas),
    Region(Point(icon.canvas_x, icon.canvas_y), Area(icon.width, icon.height)));

  return *this;
}

const IconFont &IconFont::draw(
  const var::StringView name,
  Bitmap &dest,
  const Point &point) const {
  size_t offset = find(name);
  return draw(offset, dest, point);
}
