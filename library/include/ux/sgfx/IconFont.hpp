#ifndef UXAPI_UX_SGFX_ICONFONT_HPP
#define UXAPI_UX_SGFX_ICONFONT_HPP

#include <fs/File.hpp>

#include "Font.hpp"

namespace ux::sgfx {

class IconFont;

/*! \brief IconInfo Class
 * \details The IconInfo class
 * contains details of an icon that
 * is stored within an icon font.
 *
 */

class IconFont : public api::ExecutionContext, public BitmapFlags {
public:
  using FontInfo = Font::Info;

  class IconInfo {
  public:
    IconInfo(const sg_font_icon_t &icon) : m_icon(icon) {}
    Area area() const { return Area(m_icon.width, m_icon.height); }
    const var::StringView  name() const { return var::StringView(m_icon.name); }
    u32 canvas_idx() const { return m_icon.canvas_idx; }
    Point canvas_point() const {
      return Point(m_icon.canvas_x, m_icon.canvas_y);
    }

    const sg_size_t width() const { return m_icon.width; }
    const sg_size_t height() const { return m_icon.height; }

  private:
    sg_font_icon_t m_icon = {0};
  };

  explicit IconFont(const fs::File &file);

  IconFont &refresh();

  sg_size_t point_size() const { return m_header.max_height; }

  BitsPerPixel bits_per_pixel() const {
    return static_cast<BitsPerPixel>(m_header.bits_per_pixel);
  }

  sg_size_t width() const { return m_header.max_width; }
  sg_size_t height() const { return m_header.max_height; }

  Area area() const { return Area(m_header.max_width, m_header.max_height); }

  const IconInfo get_info(size_t offset) const {
    return IconInfo(m_list.at(offset % m_list.count()));
  }

  size_t find(const var::StringView  name) const;

  size_t count() const { return m_list.count(); }

  const IconFont &
  draw(const var::StringView  name, Bitmap &dest, const Point &point) const;

  const IconFont &draw(size_t offset, Bitmap &dest, const Point &point) const;

private:
  mutable s32 m_master_canvas_idx = -1;
  sg_font_icon_header_t m_header = {0};
  BitmapData m_master_canvas;
  var::Vector<sg_font_icon_t> m_list;
  const fs::File &m_file;
};

} // namespace ux::sgfx

#endif // UXAPI_UX_SGFX_ICONFONT_HPP
