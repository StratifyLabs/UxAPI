// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#ifndef UXAPI_UX_DISPLAY_HPP_
#define UXAPI_UX_DISPLAY_HPP_

#include <sdk/types.h>
#include <sos/dev/display.h>

#include <var/View.hpp>
#include <var/StringView.hpp>

#include "sgfx/Bitmap.hpp"

namespace ux {

class Display : public fs::FileMemberAccess<Display, fs::File>,
                public api::ExecutionContext {
public:
  enum class Mode { palette, raw };
  using BitsPerPixel = sgfx::Bitmap::BitsPerPixel;

  class Info {
  public:
    Info() = default;
    Info(const display_info_t &info) : m_info(info) {}

    bool is_valid() const { return memory_size() != 0; }

    sg_size_t width() const { return m_info.width; }
    sg_size_t height() const { return m_info.height; }

    sg_size_t margin_right() const { return m_info.margin_right; }
    sg_size_t margin_left() const { return m_info.margin_left; }
    sg_size_t margin_top() const { return m_info.margin_top; }
    sg_size_t margin_bottom() const { return m_info.margin_bottom; }

    BitsPerPixel bits_per_pixel() const {
      return static_cast<BitsPerPixel>(m_info.bits_per_pixel);
    }

    u32 memory_size() const {
      // number of bytes needed to store an image
      return (width() * height() * static_cast<int>(bits_per_pixel())) / 8;
    }

    u32 frequency() const { return m_info.freq; }

    u16 columns() const { return m_info.cols; }
    u16 rows() const { return m_info.rows; }

  private:
    display_info_t m_info = {};
  };

  Display(var::StringView path);

  Display::Info get_info() const;

  const Display &write_bitmap(const sgfx::Bitmap &bitmap) const;
  Display &write_bitmap(const sgfx::Bitmap &bitmap) {
    return API_CONST_CAST_SELF(Display, write_bitmap, bitmap);
  }

  // const Display &write(const sgfx::Bitmap &bitmap) const;
  sgfx::Region region() const { return sgfx::Region(); }
  const Display &set_window(const sgfx::Region &region) const;
  const Display &set_mode(Mode value) const;
  const Display &enable() const;
  const Display &clear() const;
  const Display &disable() const;
  const Display &refresh() const;
  const Display &
  wait(const chrono::MicroTime &resolution = 10_milliseconds) const;
  bool is_busy() const;
  const Display &set_palette(const sgfx::Palette &palette) const;
  sgfx::Palette get_palette() const;

protected:

private:
  API_AC(Display, sgfx::Bitmap, bitmap);
};

} // namespace ux

#endif /* UXAPI_UX_DISPLAY_HPP_ */
