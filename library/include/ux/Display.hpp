/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#ifndef UXAPI_UX_DISPLAY_HPP_
#define UXAPI_UX_DISPLAY_HPP_

#include <mcu/types.h>
#include <sos/dev/display.h>

#include <var/View.hpp>

#include "sgfx/Bitmap.hpp"

namespace ux::sgfx {

class Display : public sgfx::Bitmap {
public:
  class Info {
  public:
    Info() { m_info = {0}; }
    Info(const display_info_t &info) : m_info(info) {}

    bool is_valid() const { return memory_size() != 0; }

    sg_size_t width() const { return m_info.width; }
    sg_size_t height() const { return m_info.width; }

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
    display_info_t m_info;
  };

  /*! \details Constructs a new object and dynamically allocates memory for
   * the buffer.
   *
   * @param w The width of the display
   * @param h The height of the display
   */
  Display(var::View view, const Area &area, BitsPerPixel bpp)
    : sgfx::Bitmap(view, area, bpp) {}

  Display(const BitmapData &bitmap_data) : Bitmap(bitmap_data) {}

  const Display &set_window(const sgfx::Region &region) const {
    API_RETURN_VALUE_IF_ERROR(*this);
    API_SYSTEM_CALL("", interface_set_window(region));
    return *this;
  }

  const Display &write(const sgfx::Bitmap &bitmap) const {
    API_RETURN_VALUE_IF_ERROR(*this);
    API_SYSTEM_CALL("", interface_write(bitmap));
    return *this;
  }

  const Display &enable() const {
    API_RETURN_VALUE_IF_ERROR(*this);
    API_SYSTEM_CALL("", interface_enable());
    return *this;
  }

  const Display &clear() const {
    API_RETURN_VALUE_IF_ERROR(*this);
    API_SYSTEM_CALL("", interface_clear());
    return *this;
  }

  const Display &disable() const {
    API_RETURN_VALUE_IF_ERROR(*this);
    API_SYSTEM_CALL("", interface_disable());
    return *this;
  }

  virtual Info get_info() const { return Info(); }
  virtual sgfx::Palette get_palette() const { return sgfx::Palette(); }

  virtual int set_palette(const sgfx::Palette &palette) const {
    MCU_UNUSED_ARGUMENT(palette);
    return -1;
  }

protected:
  virtual int interface_enable() const = 0;
  virtual int interface_disable() const = 0;

  virtual int interface_set_window(const sgfx::Region &region) const {
    return 0;
  }
  virtual int interface_write(const sgfx::Bitmap &bitmap) const { return 0; }
  virtual int interface_clear() const { return 0; }
};

template <class Derived> class DisplayAccess : public Display {
public:
  DisplayAccess(var::View view, const Area &area, BitsPerPixel bpp)
    : Display(view, area, bpp) {}

  DisplayAccess(const BitmapData &bitmap_data) : Display(bitmap_data) {}

  const DisplayAccess &set_window(const sgfx::Region &region) const {
    return static_cast<Derived &>(Display::set_window(region));
  }

  const DisplayAccess &write(const sgfx::Bitmap &bitmap) const {
    return static_cast<Derived &>(Display::write(bitmap));
  }

  const DisplayAccess &enable() const {
    return static_cast<Derived &>(Display::enable());
  }

  const DisplayAccess &disable() const {
    return static_cast<Derived &>(Display::disable());
  }
};

} // namespace ux::sgfx

#endif /* UXAPI_UX_DISPLAY_HPP_ */
