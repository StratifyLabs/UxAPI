// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#include "ux/Layout.hpp"
#include "ux/EventLoop.hpp"

using namespace ux::sgfx;
using namespace ux;

Layout::Layout(const var::StringView name) : ComponentAccess(name) {
  m_origin = DrawingPoint(0, 0);
  set_align_left();
  set_align_top();
  set_layout();
}

Layout::~Layout() {
  for (Item &item : m_component_list) {
    if (item.component() && item.component()->is_created()) {
      delete item.component();
    }
  }
}

void Layout::examine_visibility() {
  if (is_ready_to_draw()) {
    if (is_created() == false) {
      printf(
        "fatal %s was note created using Component::create()\n",
        var::IdString(name()).cstring());
      exit(1);
    }

    if (display() == nullptr) {
      set_visible_examine(false);
      return;
    }

    m_reference_drawing_attributes.set_bitmap(display()->bitmap());

    m_refresh_region = Region(
      Point(0, 0),
      Region(reference_drawing_attributes().calculate_region_on_bitmap())
        .area());

    shift_origin(DrawingPoint(0, 0));

    handle_event(SystemEvent(SystemEvent::event_id_enter));

  } else {

    // if layout is not ready -- components are not visible
    for (Item &component_pointer : m_component_list) {
      if (component_pointer.component()) {
        component_pointer.component()->set_visible_examine(false);
      }
    }

    handle_event(SystemEvent(SystemEvent::event_id_exit));
  }
}

void Layout::set_refresh_region(const sgfx::Region &region) {
  if (is_ready_to_draw()) {
    sgfx::Region layout_region
      = reference_drawing_attributes().calculate_region_on_bitmap();

    layout_region
      = Region(layout_region.point() + region.point(), region.area());

    for (Item &component_pointer : m_component_list) {
      if (component_pointer.component()) {
        sgfx::Region component_region = component_pointer.component()
                                          ->reference_drawing_attributes()
                                          .calculate_region_on_bitmap();

        component_pointer.component()->set_refresh_region(
          layout_region.overlap(component_region));
      }
    }
  }
  m_refresh_region = region;
}

void Layout::erase_region(DrawingRegion region) {
  const auto attributes
    = reference_drawing_attributes() + region.point() + region.area();
  const sgfx::Region window_region = attributes.calculate_region_on_bitmap();

  theme()->set_display_palette(*display(), m_theme_style, m_theme_state);

  if ((window_region.width() * window_region.height()) > 0) {
    display()->set_window(window_region);
    display()->clear();
  }
}

Layout &Layout::add_component(Component &component) {

  component.set_parent(this);

  // check to see if a cp has been deleted -- insert the new component there
  for (Item &item : m_component_list) {
    if (item.component() == nullptr) {
      item = Item(&component);
      examine_visibility();
      return *this;
    }
  }

  m_component_list.push_back(Item(&component));
  return *this;
}

Layout &Layout::delete_component(const var::StringView component_name) {

  for (Item &item : m_component_list) {
    if (item.component() && (component_name == item.component()->name())) {
      item.component()->set_enabled_examine(false);
      API_ASSERT(item.component()->is_busy() == false);
      delete item.component();
      item.set_component(nullptr);
      break;
    }
  }

  return *this;
}

void Layout::update_drawing_point(
  const Component *component,
  const DrawingPoint &point) {
  for (Item &item : m_component_list) {
    if (item.component() && (component == item.component())) {
      item.set_drawing_point(point);
      shift_origin(DrawingPoint(0, 0));
      return;
    }
  }
}

void Layout::update_drawing_area(
  const Component *component,
  const DrawingArea &area) {
  for (Item &item : m_component_list) {
    if (item.component() && component == item.component()) {
      item.set_drawing_area(area);
      shift_origin(DrawingPoint(0, 0));
      return;
    }
  }
}

void Layout::shift_origin(DrawingPoint shift) {
  m_origin += shift;

  if (is_ready_to_draw()) {
    // determine scroll ends
    generate_layout_positions();

    const auto layout_region = Region(
      Point(reference_drawing_attributes().calculate_region_on_bitmap().point) + m_refresh_region.point(),
      m_refresh_region.area());

    for (Item &item : m_component_list) {
      if (item.component()) {
        // reference attributes are the location within the compound component
        // translate reference attributes based on compound component attributes
        item.component()->reference_drawing_attributes()
          = reference_drawing_attributes() + m_origin + item.drawing_point()
            + item.drawing_area();

        const auto component_region = item.component()
                                          ->reference_drawing_attributes()
                                          .calculate_region_on_bitmap();

        const auto overlap = layout_region.overlap(component_region);

        if (overlap.width() * overlap.height() > 0) {
          // this calculates if only part of the element should be refreshed
          // (the mask)

          item.component()->set_refresh_drawing_pending();

          if (item.component()->is_visible()) {
            item.component()->touch_drawing_attributes();
          } else {
            item.component()->set_visible_examine(true);
          }

          item.component()->set_refresh_region(overlap);

        } else {
          //no overlap with the layout view, so component is disabled
          item.component()->set_visible_examine(false);
        }
      }
    }
  }
}

void Layout::scroll(DrawingPoint value) {
  if (is_ready_to_draw()) {
    shift_origin(value);
  }
}

void Layout::draw(const DrawingAttributes &attributes) {
  MCU_UNUSED_ARGUMENT(attributes);
  for (const Item &item : m_component_list) {
    if (item.component() && item.component()->is_focus()) {
      item.component()->redraw();
    }
  }
}

Layout &Layout::set_focus(bool value) {
  set_focus_internal(value);
  for (const Item &item : m_component_list) {
    if (item.component()) {
      if (item.component()->is_layout()) {
        item.component()->reinterpret<Layout>()->set_focus(value);
      } else {
        item.component()->set_focus_internal(value);
      }
    }
  }
  return *this;
}

void Layout::distribute_event(const ux::Event &event) {
  // handle scrolling -- pass events to specific components
  set_busy();
  if (is_focus()) {

    auto * const touch_context = event.is_trigger<TouchContext>();

    if (
      touch_context && event.id() == TouchContext::event_id_pressed
      && contains(touch_context->point())) {
      set_touch_focus(true);
    }

    if (touch_context && event.id() == TouchContext::event_id_released) {
      set_touch_focus(false);
    }

    if (
      touch_context && event.id() == TouchContext::event_id_dragged_point
      && is_touch_focus()) {

      drawing_int_t vertical_drawing_scroll;
      drawing_int_t horizontal_drawing_scroll;
      horizontal_drawing_scroll
        = handle_horizontal_scroll(touch_context->drag().x());

      vertical_drawing_scroll
        = handle_vertical_scroll(touch_context->drag().y());

      if (vertical_drawing_scroll || horizontal_drawing_scroll) {

        touch_context->set_point(
          Point(vertical_drawing_scroll, horizontal_drawing_scroll));

        this->scroll(
          DrawingPoint(horizontal_drawing_scroll, vertical_drawing_scroll));
      }
    }

    handle_event(event);
    for (Item &item : m_component_list) {
      if (
        item.component() && item.component()->is_enabled()
        && item.component()->is_focus()) {
        if (item.component()->is_layout()) {
          item.component()->reinterpret<Layout>()->distribute_event(event);
        } else {
          item.component()->handle_event(event);
        }
      }
    }
  }

  if (event == SystemEvent(SystemEvent::event_id_exit)) {
    for (Item &component_pointer : m_component_list) {
      if (component_pointer.component()) {
        component_pointer.component()->set_visible_examine(false);
      }
    }
  }

  set_busy(false);
}

drawing_int_t Layout::handle_vertical_scroll(sg_int_t scroll) {
  // convert scroll to drawing scale

  drawing_int_t drawing_scroll
    = scroll * Drawing::scale()
      / reference_drawing_attributes().calculate_height_on_bitmap();

  if (drawing_scroll < 0) {

    drawing_int_t max_scroll = m_area.height() - 1000 + m_origin.y();
    if (drawing_scroll * -1 > max_scroll) {
      drawing_scroll = -1 * max_scroll;
    }

    if (m_origin.y() + m_area.height() <= 1000) {
      drawing_scroll = 0;
    }

  } else {
    if (drawing_scroll > m_origin.y() * -1) {
      drawing_scroll = m_origin.y() * -1;
    }
  }

  return drawing_scroll;
}

drawing_int_t Layout::handle_horizontal_scroll(sg_int_t scroll) { return 0; }

void Layout::generate_layout_positions() {
  switch (m_flow) {
  default:
  case Flow::free:
    generate_free_layout_positions();
    return;
  case Flow::vertical:
    generate_vertical_layout_positions();
    return;
  case Flow::vertical_no_scroll:
    generate_vertical_no_scroll_layout_positions();
    return;
  case Flow::horizontal:
    generate_horizontal_layout_positions();
    return;
  }
}

void Layout::generate_free_layout_positions() {
  drawing_int_t x_max = 0;
  drawing_int_t y_max = 0;

  for (Item &item : m_component_list) {
    if (
      item.component()
      && (item.drawing_point().x() + item.drawing_area().width() > x_max)) {
      x_max = item.drawing_point().x() + item.drawing_area().width();
    }

    if (
      item.component()
      && (item.drawing_point().y() + item.drawing_area().height() > y_max)) {
      y_max = item.drawing_point().y() + item.drawing_area().height();
    }
  }

  m_area = DrawingArea(x_max, y_max);
}

void Layout::generate_vertical_layout_positions() {
  drawing_int_t drawing_cursor = 0;
  sg_int_t bitmap_cursor = 0;

  for (Item &item : m_component_list) {
    if (item.component()) {
      const DrawingPoint point(0, drawing_cursor);
      const DrawingArea area(1000, item.drawing_area().height());
      item.set_drawing_point(point);

      DrawingAttributes tmp_attributes
        = reference_drawing_attributes() + m_origin + point + area;

      sg_size_t height_on_bitmap
        = tmp_attributes.calculate_area_on_bitmap().height;

      bitmap_cursor += height_on_bitmap - 1;

      drawing_cursor
        = reference_drawing_attributes().calculate_height_on_drawing(
          bitmap_cursor);
    }
  }

  m_area.set_height(drawing_cursor);
}

void Layout::generate_vertical_no_scroll_layout_positions() {
  drawing_int_t drawing_cursor = 0;

  for (Item &item : m_component_list) {
    if (item.component()) {
      item.set_drawing_point(DrawingPoint(0, drawing_cursor));
      drawing_cursor += item.drawing_area().height();
    }
  }

  m_area.set_height(drawing_cursor);
}

void Layout::generate_horizontal_layout_positions() {
  drawing_int_t drawing_cursor = 0;
  sg_int_t bitmap_cursor = 0;

  for (Item &component : m_component_list) {
    if (component.component()) {

      const DrawingPoint point(drawing_cursor, 0);
      const DrawingArea area(component.drawing_area().width(), 1000);
      component.set_drawing_point(point);

      DrawingAttributes tmp_attributes = reference_drawing_attributes() + area;
      sg_size_t width_on_bitmap
        = tmp_attributes.calculate_area_on_bitmap().width;

      bitmap_cursor += width_on_bitmap - 1;
      drawing_cursor
        = reference_drawing_attributes().calculate_width_on_drawing(
          bitmap_cursor);
    }
  }

  m_area.set_width(drawing_cursor);
}
