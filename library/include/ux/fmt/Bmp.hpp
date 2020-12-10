/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#ifndef SAPI_FMT_BMP_HPP_
#define SAPI_FMT_BMP_HPP_

#include <fs/File.hpp>
#include <sdk/types.h>

#include "../sgfx/Bitmap.hpp"

namespace ux::fmt {

/*! \brief BMP File format */
class Bmp {
public:
  using BitsPerPixel = ux::sgfx::Bitmap::BitsPerPixel;

  /*! \details Creates a new bitmap using the specified parameters. */
  class Construct {
    API_AF(Construct, const fs::FileObject *, file_object, nullptr);
    API_AF(Construct, sg_int_t, width, 0);
    API_AF(Construct, sg_int_t, height, 0);
    API_AF(Construct, sg_int_t, plane_count, 0);
    API_AF(Construct, BitsPerPixel, bits_per_pixel, BitsPerPixel::x1);
  };

  explicit Bmp(const Construct &options);

  /*! \details Constructs a new bitmap object and opens the bitmap as a
   * read-only file. */
  explicit Bmp(const fs::FileObject *file_object);

  /*! \details Constructs an empty bitmap object. */
  Bmp();

  static void save(
    const fs::FileObject &file,
    const ux::sgfx::Bitmap &bitmap,
    const ux::sgfx::Palette &pallete);

  ux::sgfx::Bitmap convert_to_bitmap(BitsPerPixel bpp);

  /*! \details Returns the bitmap width (after bitmap has been opened). */
  s32 width() const { return m_dib.width; }
  /*! \details Returns the bitmap height (after bitmap has been opened). */
  s32 height() const { return m_dib.height; }
  /*! \details Returns the bitmap bits per pixel (after bitmap has been opened).
   */
  u16 bits_per_pixel() const { return m_dib.bits_per_pixel; }
  /*! \details Returns the bitmap planes (after bitmap has been opened). */
  u16 planes() const { return m_dib.planes; }

  /*! \details Calculates the bytes needed to store one row of data (after
   * bitmap has been opened). */
  unsigned int calculate_row_size() const;
  unsigned int calc_row_size() const { return calculate_row_size(); }

  /*! \details Moves file pointer to the start of the bitmap data. */
  Bmp &rewind() {
    m_file_object->seek(m_offset);
    return *this;
  }

  /*! \details Seeks the file to the data at the specified row. */
  const Bmp &seek_row(s32 y) const;

  class ReadPixel {
    API_AB(ReadPixel, mono, false);
    API_AF(ReadPixel, u8, threshold, 128);
  };

  const Bmp &read_pixel(var::View pixel) const;
  bool to_mono(const var::View pixel, u8 threshold = 128) const;

  typedef struct MCU_PACK {
    u16 signature;
    u32 size;
    u16 resd1;
    u16 resd2;
    u32 offset;
  } bmp_header_t;

  typedef struct MCU_PACK {
    u32 hdr_size;
    s32 width;
    s32 height;
    u16 planes;
    u16 bits_per_pixel;
  } bmp_dib_t;

  static constexpr u16 signature() { return 0x4D42; }

private:
  bmp_dib_t m_dib;
  u32 m_offset;
  const fs::FileObject *m_file_object;
  void load();
};

} // namespace ux::fmt

#endif /* SAPI_FMT_BMP_HPP_ */
