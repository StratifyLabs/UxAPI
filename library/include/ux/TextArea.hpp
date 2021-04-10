// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#ifndef UXAPI_UX_TEXTAREA_HPP
#define UXAPI_UX_TEXTAREA_HPP

#include "Component.hpp"
#include "TouchGesture.hpp"
#include "draw/TextBox.hpp"

namespace ux {

class TextArea : public ComponentAccess<TextArea> {
public:

  enum event_id {
    event_id_none,
    event_id_scroll_incremented,
    event_id_scroll_decremented
  };

  enum class Encoding {
    raw,
    binary,
    hexadecimal
  };

  class Construct {
    API_ACCESS_FUNDAMENTAL(Construct, size_t, content_size, 0);
    API_ACCESS_FUNDAMENTAL(Construct, const fs::FileObject *, source, nullptr);
    API_ACCESS_FUNDAMENTAL(Construct, Encoding, encoding, Encoding::raw);
    API_AC(Construct, var::View, page_buffer);
  };

  TextArea(const var::StringView name, const Construct &options);

  void draw(const DrawingScaledAttributes &attributes);
  void handle_event(const ux::Event &event);

  TextArea &scroll_top() {
    m_text_box.set_scroll(0);
    return *this;
  }

  TextArea &scroll_bottom() {
    m_text_box.set_scroll(draw::TextBox::maximum_scroll());
    return *this;
  }

  TextArea & set_display_text(const var::StringView value){
    m_display_text = value;
    return *this;
  }

  size_t scroll() const {
    return m_scroll;
  }

  size_t scroll_total() const {
    return m_scroll_total;
  }


private:
  static constexpr size_t m_scroll_total = 50;

  API_ACCESS_FUNDAMENTAL(TextArea, drawing_size_t, text_height, 0);
  API_ACCESS_FUNDAMENTAL(TextArea, drawing_size_t, offset, 0);

  const fs::FileObject *m_source = nullptr;
  size_t m_content_location = 0;
  size_t m_content_size = 0;
  var::View m_page_buffer;
  size_t m_display_width = 0;
  size_t m_page_start = 0;
  sg_int_t m_scroll_accumulator = 0;
  sg_size_t m_line_height = 0;
  draw::TextBox m_text_box;
  var::StringView m_display_text;
  size_t m_scroll = 0;
  bool m_is_text_box_scroll_mode = false;

  size_t calculate_remaining() const {
    const auto content_size = (m_content_location + m_content_size) - m_source->location();
    return content_size > m_page_buffer.size() ? m_page_buffer.size() : content_size;
  }

  void load_page_buffer();
  void handle_increment_scroll();
  void handle_decrement_scroll();
};

} // namespace ux

#endif // UXAPI_UX_TEXTAREA_HPP
