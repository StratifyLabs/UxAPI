// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#include <fs.hpp>
#include <var.hpp>

#include "ux/TextArea.hpp"
#include "ux/TouchGesture.hpp"
#include "ux/draw/TextBox.hpp"

using namespace ux::sgfx;
using namespace ux;

TextArea::TextArea(const var::StringView name, const Construct &options)
  : ComponentAccess(name), m_source(options.source()) {

  if (m_source != nullptr) {
    m_page_buffer = options.page_buffer();
    m_content_location = m_source->location();
    m_content_size = options.content_size() ? options.content_size()
                                            : m_source->size_signed();
    m_page_start = m_content_location;
    load_page_buffer();
  } else {
    m_display_text = StringView(m_page_buffer.to_char(), m_page_buffer.size());
  }
}

void TextArea::draw(const DrawingScaledAttributes &attributes) {
  draw_base_properties(attributes.bitmap(), attributes.region(), theme());

  Region region_inside_padding
    = calculate_region_inside_padding(attributes.region());

  set_model(value());

  // calculate the font point size scaled from drawing size to sgfx size
  m_line_height = text_height() * region_inside_padding.height() * 9 / 10000;

  // if the label is available, draw it

  const auto draw_attributes
    = attributes + region_inside_padding.point() + region_inside_padding.area();

  m_text_box.set_font(nullptr)
    .set_string(m_display_text)
    .set_font_name(
      font_name().is_empty() ? theme()->primary_font_name() : font_name())
    .set_font_point_size(m_line_height)
    .set_color(theme()->text_color())
    .set_alignment(alignment())
    .draw(draw_attributes);

  m_display_width = draw_attributes.area().width();

  apply_antialias_filter(attributes);
}

void TextArea::handle_event(const ux::Event &event) {
  const TouchContext *touch_context = event.is_trigger<TouchContext>();
  if (touch_context) {
    if (event.id() == TouchContext::event_id_dragged_point) {
      if (contains(touch_context->point())) {

        bool is_increment = false;
        bool is_decrement = false;
        const sg_size_t line_compare = m_line_height / 2;
        m_scroll_accumulator += touch_context->drag().y();

        while (m_scroll_accumulator > line_compare) {
          m_scroll_accumulator -= line_compare;
          handle_decrement_scroll();
          is_decrement = true;
        }

        while (m_scroll_accumulator < -1 * line_compare) {
          m_scroll_accumulator += line_compare;
          handle_increment_scroll();
          is_increment = true;
        }

        if (is_decrement) {
          trigger_event(event_id_scroll_decremented);
        } else if (is_increment) {
          trigger_event(event_id_scroll_incremented);
        }

        if (is_decrement || is_increment) {
          redraw();
        }
      }

    } else if (event.id() == TouchContext::event_id_released) {
      m_scroll_accumulator = 0;
    }
  }
}

void TextArea::load_page_buffer() {
  API_ASSERT(m_source != nullptr);

  const auto bytes_read
    = ViewFile(m_page_buffer)
        .write(*m_source, File::Write().set_size(calculate_remaining()))
        .return_value();

  if (bytes_read <= 0) {
    return;
  }

  if (bytes_read < m_page_buffer.size_signed()) {
    // use text box scrolling
    m_is_text_box_scroll_mode = true;
  } else {
    m_is_text_box_scroll_mode = false;
  }

  m_scroll = ((m_page_start - m_content_location) * 25 + m_content_size / 2)
             / m_content_size;

  m_display_text = StringView(m_page_buffer.to_char(), bytes_read);
}

void TextArea::handle_increment_scroll() {
  // scroll to a place further a long in the file
  if (m_is_text_box_scroll_mode || m_source == nullptr) {
    m_text_box.increment_scroll();
    if (m_text_box.scroll() > m_text_box.scroll_total()) {
      m_scroll = m_scroll_total;
    }
    return;
  }

  m_page_start
    = m_source->seek(m_page_start + m_text_box.first_line_length() + 1)
        .location();
  load_page_buffer();
}

void TextArea::handle_decrement_scroll() {
  if (m_is_text_box_scroll_mode || m_source == nullptr) {
    if (m_text_box.scroll() > 1) {
      m_text_box.decrement_scroll();
      redraw();
      return;
    }
    if (m_source) {
      m_is_text_box_scroll_mode = false;
    } else {
      return;
    }
  }

  GeneralString line_buffer;
  const bool is_full_capacity
    = (m_page_start - m_content_location) > line_buffer.capacity();

  const auto preload_start = is_full_capacity
                               ? m_page_start - line_buffer.capacity()
                               : m_content_location;
  const auto preload_size
    = is_full_capacity ? line_buffer.capacity() : m_page_start;


  m_source->seek(preload_start)
    .read(var::View(line_buffer.data(), preload_size));
  line_buffer.data()[preload_size] = 0;

  API_ASSERT(m_text_box.font() != nullptr);
  // use text box to build out the lines
  const auto count_lines_result = draw::TextBox::count_lines(
    m_text_box.font(),
    line_buffer,
    m_display_width);

  m_page_start
    = m_page_start - m_content_location
          < count_lines_result.characters_on_last_line + 1
        ? m_content_location
        : m_page_start - count_lines_result.characters_on_last_line - 1;


  m_source->seek(m_page_start);

  load_page_buffer();
}
