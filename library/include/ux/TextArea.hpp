// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#ifndef UXAPI_UX_TEXTAREA_HPP
#define UXAPI_UX_TEXTAREA_HPP

#include "Component.hpp"
#include "TouchGesture.hpp"
#include "draw/TextBox.hpp"

namespace ux {

class TextArea : public ComponentAccess<TextArea> {
public:
  TextArea(const var::StringView name);

  void draw(const DrawingScaledAttributes &attributes);
  void handle_event(const ux::Event &event);

  TextArea &append(const var::StringView input) {
    return set_value(value() + input);
  }

  TextArea &scroll_top() {
    m_text_box.set_scroll(0);
    return *this;
  }

  TextArea &scroll_bottom() {
    m_text_box.set_scroll(draw::TextBox::maximum_scroll());
    return *this;
  }

  size_t scroll() const {
    return m_text_box.scroll();
  }

  size_t scroll_total() const {
    return m_text_box.scroll_total();
  }

  TextArea & set_display_text(const var::StringView value){
    m_display_text = value;
    return *this;
  }

  const var::StringView value() const { return get_model(); }

private:
  API_ACCESS_FUNDAMENTAL(TextArea, size_t, max_length, 256);
  API_ACCESS_FUNDAMENTAL(TextArea, drawing_size_t, text_height, 0);
  API_ACCESS_FUNDAMENTAL(TextArea, drawing_size_t, offset, 0);

  sg_int_t m_scroll_accumulator = 0;
  sg_size_t m_line_height = 0;
  draw::TextBox m_text_box;
  var::StringView m_display_text;
};

} // namespace ux

#endif // UXAPI_UX_TEXTAREA_HPP
