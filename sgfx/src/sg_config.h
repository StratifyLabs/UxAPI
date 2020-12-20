// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#ifndef SG_CONFIG_H_
#define SG_CONFIG_H_

#include <stdio.h>

#include "sgfx_types.h"

#if defined __1bpp
#define SG_BITS_PER_PIXEL 1
#elif defined __2bpp
#define SG_BITS_PER_PIXEL 2
#elif defined __4bpp
#define SG_BITS_PER_PIXEL 4
#elif defined __8bpp
#define SG_BITS_PER_PIXEL 8
#elif defined __16bpp
#define SG_BITS_PER_PIXEL 16
#else
#define SG_BITS_PER_PIXEL 0
#endif

#if SG_BITS_PER_PIXEL == 0
#define SG_BITS_PER_PIXEL_VALUE(bmap) (bmap->bits_per_pixel)
#else
#define SG_BITS_PER_PIXEL_VALUE(bmap) SG_BITS_PER_PIXEL
#endif

#define SG_BITS_PER_WORD 32
#define SG_BYTES_PER_WORD (SG_BITS_PER_WORD / 8)
#define SG_PIXELS_PER_WORD(bmap)                                               \
  (SG_BITS_PER_WORD / SG_BITS_PER_PIXEL_VALUE(bmap))
#define SG_PIXEL_MASK(bmap) ((1 << SG_BITS_PER_PIXEL_VALUE(bmap)) - 1)

sg_color_t sg_cursor_get_pixel_no_increment(sg_cursor_t *cursor);
void sg_cursor_draw_pixel_no_increment(sg_cursor_t *cursor);

#endif /* SG_CONFIG_H_ */
