// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#include "ux/Progress.hpp"
#include "ux/draw/Rectangle.hpp"

using namespace ux::sgfx;
using namespace ux;

ProgressBar::Value ProgressBar::value_from_string(const var::StringView value){
  auto list = value.split("/");
  Value result;
  result.at(0) = list.count() ? list.at(0).to_unsigned_long() : 0;
  result.at(1) = list.count() > 1 ? list.at(1).to_unsigned_long() : 100;
  return result;
}

ProgressBar::Value ProgressBar::value() const {
  return value_from_string(get_model());
}

void ProgressBar::draw(const DrawingScaledAttributes &attributes) {

  // draw the Border
  draw_base_properties(attributes.bitmap(), attributes.region(), theme());

  Region region_inside_padding
    = calculate_region_inside_padding(attributes.region());

  const u16 progress_width = [](u16 width) {
    u16 result = width;
    if (result < 3) {
      result = 3;
    }

    if (result > 95) {
      result = 95;
    }
    return result;
  }(progress().width());


  // calculate the width
  const bool is_indeterminate = (progress().maximum() == 0);
  const sg_size_t width = is_indeterminate
                      ? region_inside_padding.width() / progress_width
                      : region_inside_padding.width() * progress().value()
                          / progress().maximum();

  sg_int_t x_offset = is_indeterminate ? region_inside_padding.width()
                                           * (progress().value() % 100) / 100
                                       : 0;

  if (x_offset + width > region_inside_padding.width()) {
    x_offset = region_inside_padding.width() - width;
  }

  Region progress_region = Region(region_inside_padding)
                             .set_width(width)
                             .set_x(region_inside_padding.x() + x_offset);

  draw::Rectangle()
    .set_color(theme()->text_color())
    .draw(attributes + progress_region.point() + progress_region.area());

  apply_antialias_filter(attributes);
}
