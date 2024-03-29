/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#ifndef UXAPI_UX_SGFX_FILEFONT_HPP_
#define UXAPI_UX_SGFX_FILEFONT_HPP_

#include "Font.hpp"
#include <fs/File.hpp>

namespace ux::sgfx {

/*! \brief File Font Class
 * \details The File Font class is used to access
 * fonts that are stored as files.
 *
 */
class FileFont : public Font {
public:
  FileFont();
  FileFont(const var::StringView name, FileLocation offset = FileLocation(0));

  ~FileFont();

  int set_file(const var::StringView path, FileLocation offset = FileLocation(0));

  sg_size_t get_height() const;
  sg_size_t get_width() const;

protected:
  void draw_char_on_bitmap(
    const sg_font_char_t &ch,
    Bitmap &dest,
    const Point &point) const;
  int load_char(sg_font_char_t &ch, char c, bool ascii) const;
  int load_kerning(u16 first, u16 second) const;
  sg_font_kerning_pair_t load_kerning(u32 offset) const;

private:
  mutable fs::File m_file;
  mutable Bitmap m_canvas;
  mutable u8 m_current_canvas;
  u32 m_canvas_start;
  u32 m_canvas_size;
  sg_font_kerning_pair_t *m_kerning_pairs;
};

} // namespace ux::sgfx

#endif /* UXAPI_UX_SGFX_FILEFONT_HPP_ */
