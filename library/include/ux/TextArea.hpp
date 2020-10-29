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

  TextArea &set_value(const var::StringView  value);

  const var::StringView value() const { return lookup_model_value(); }

private:
  API_ACCESS_FUNDAMENTAL(TextArea, size_t, max_length, 256);
  API_ACCESS_FUNDAMENTAL(TextArea, drawing_size_t, text_height, 0);
  API_ACCESS_FUNDAMENTAL(TextArea, drawing_size_t, offset, 0);

  sg_int_t m_scroll_accumulator = 0;
  sg_size_t m_line_height = 0;
  draw::TextBox m_text_box;
  var::StringView get_display_text();
};

} // namespace ux

#endif // UXAPI_UX_TEXTAREA_HPP
