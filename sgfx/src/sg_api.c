// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#include "sg_config.h"
#include "sgfx.h"

#if !defined SOS_GIT_HASH
#define SOS_GIT_HASH "??"
#endif

const sg_api_t sg_api = {
  .sos_api = {.name = "sgfx", .version = SG_VERSION, .git_hash = SOS_GIT_HASH},
  .bits_per_pixel = SG_BITS_PER_PIXEL,
  .bmap_set_data = sg_bmap_set_data,
  .bmap_data = sg_bmap_data,
  .calc_bmap_size = sg_calc_bmap_size,

  // point operations
  .point_set = sg_point_set,
  .point_map = sg_point_map,
  .point_unmap = sg_point_unmap,
  .point_map_pixel_size = sg_point_map_pixel_size,
  .point_add = sg_point_add,
  .point_subtract = sg_point_subtract,
  .point_arc = sg_point_arc,
  .point_rotate = sg_point_rotate,
  .point_scale = sg_point_scale,
  .point_shift = sg_point_shift,
  .point_shift_x = sg_point_shift_x,
  .point_shift_y = sg_point_shift_y,
  .point_bound = sg_point_bound,
  .point_bound_x = sg_point_bound_x,
  .point_bound_y = sg_point_bound_y,

  // transforms
  .transform_flip_x = sg_transform_flip_x,
  .transform_flip_y = sg_transform_flip_y,
  .transform_flip_xy = sg_transform_flip_xy,
  .transform_shift = sg_transform_shift,

  // cursor
  .cursor_set = sg_cursor_set,
  .cursor_update = sg_cursor_update,
  .cursor_get_pixel_increment = sg_cursor_get_pixel_increment,
  .cursor_inc_x = sg_cursor_inc_x,
  .cursor_dec_x = sg_cursor_dec_x,
  .cursor_inc_y = sg_cursor_inc_y,
  .cursor_dec_y = sg_cursor_dec_y,
  .cursor_get_pixel = sg_cursor_get_pixel,
  .cursor_draw_pixel = sg_cursor_draw_pixel,
  .cursor_draw_hline = sg_cursor_draw_hline,
  .cursor_draw_cursor = sg_cursor_draw_cursor,
  .cursor_draw_pattern = sg_cursor_draw_pattern,
  .cursor_shift_right = sg_cursor_shift_right,
  .cursor_shift_left = sg_cursor_shift_left,
  .cursor_find_positive_edge = sg_cursor_find_positive_edge,
  .cursor_find_negative_edge = sg_cursor_find_negative_edge,

  // drawing
  .get_pixel = sg_get_pixel,
  .draw_pixel = sg_draw_pixel,
  .draw_line = sg_draw_line,
  .draw_quadratic_bezier = sg_draw_quadratic_bezier,
  .draw_cubic_bezier = sg_draw_cubic_bezier,
  .draw_rectangle = sg_draw_rectangle,
  .draw_arc = sg_draw_arc,
  .draw_pour = sg_draw_pour,
  .draw_pattern = sg_draw_pattern,
  .draw_bitmap = sg_draw_bitmap,
  .draw_sub_bitmap = sg_draw_sub_bitmap,

  // vector drawing
  .vector_draw_path = sg_vector_draw_path,

  // animations
  .animate = sg_animate,
  .animate_init = sg_animate_init,

  .antialias_filter_init = sg_antialias_filter_init,
  .antialias_filter_apply = sg_antialias_filter_apply

};
