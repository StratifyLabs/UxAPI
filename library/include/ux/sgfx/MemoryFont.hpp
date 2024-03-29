/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md for rights.

#ifndef UXAPI_UX_SGFX_MEMORYFONT_HPP_
#define UXAPI_UX_SGFX_MEMORYFONT_HPP_

#include "Font.hpp"

namespace ux::sgfx {

class API_DEPRECATED_NO_REPLACEMENT MemoryFont;

class MemoryFont: public Font {
public:

	MemoryFont();

	MemoryFont(const void * ptr);
	void set_font_memory(const void * ptr);
	const void * font_memory() const { return m_font; }


	sg_size_t get_height() const;
	sg_size_t get_width() const;

protected:

	void draw_char_on_bitmap(const sg_font_char_t & ch, Bitmap & dest, const Point & point) const;
	int load_char(sg_font_char_t & ch, char c, bool ascii) const;
	int load_kerning(u16 first, u16 second) const;

private:

	int load_char_ptr(sg_font_char_t & ch, char c, bool ascii) const;
	int load_bitmap_ptr(const sg_font_char_t & ch) const;

	const void * m_font;
	mutable Bitmap m_canvas;

	u32 m_canvas_start;
	u32 m_canvas_size;

};

}

#endif /* UXAPI_UX_SGFX_MEMORYFONT_HPP_ */
