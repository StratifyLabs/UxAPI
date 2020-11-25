/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#include "ux/Component.hpp"
#include "ux/EventLoop.hpp"
#include "ux/Layout.hpp"
#include "ux/Model.hpp"

using namespace ux::sgfx;
using namespace ux;

EventLoop *Component::m_event_loop = nullptr;

Component::~Component() { set_visible_examine(false); }

void Component::examine_visibility() {
  if (is_ready_to_draw()) {
    API_ASSERT(is_created());
    API_RETURN_IF_ERROR();

    if (display() == nullptr) {
      set_visible(false);
      return;
    }

    m_reference_drawing_attributes.set_bitmap(display()->bitmap());

    // local bitmap is a small section of the reference bitmap
    m_reference_drawing_attributes.calculate_area_on_bitmap();

    m_local_bitmap_data.resize(
      m_reference_drawing_attributes.calculate_area_on_bitmap(),
      m_reference_drawing_attributes.bitmap().bits_per_pixel());

    API_ASSERT(is_success());

    m_local_bitmap = Bitmap(m_local_bitmap_data);

    // local attributes fill local bitmap
    m_local_drawing_attributes.set_area(DrawingArea(1000, 1000))
      .set_bitmap(m_local_bitmap);

    set_refresh_region(m_local_bitmap.region());
    redraw();
    handle_event(SystemEvent(SystemEvent::event_id_enter));
  } else {
    handle_event(SystemEvent(SystemEvent::event_id_exit));
    m_local_bitmap_data = BitmapData();
  }
}

void Component::set_refresh_drawing_pending() {
  m_flags |= (flag_refresh_drawing_pending);
  if (parent()) {
    parent()->set_refresh_drawing_pending();
  }
}

DrawingPoint Component::translate_point(const sgfx::Point &point) {
  if (is_ready_to_draw()) {

    sgfx::Point relative_point
      = point - m_reference_drawing_attributes.calculate_point_on_bitmap();

    sgfx::Area area = m_reference_drawing_attributes.calculate_area_on_bitmap();

    // now scale for width
    return DrawingPoint(
      1000 * relative_point.x() / area.width(),
      1000 * relative_point.y() / area.height());
  }

  return DrawingPoint(0, 0);
}

void Component::refresh_drawing() {
  if (is_ready_to_draw()) {
    // use the palette if it is available
    theme()->set_display_palette(*display(), m_theme_style, m_theme_state);

    Region window_region = Region(
      Point(m_reference_drawing_attributes.calculate_point_on_bitmap())
        + m_refresh_region.point(),
      m_refresh_region.area());

    if (window_region.width() * window_region.height() > 0) {
      display()->set_window(window_region);

#if 0
      printer::Printer p;
      p.object("draw " + name(), window_region);
      p.object("refreshRegion " + name(), m_refresh_region);
#endif

      display()->write_bitmap(
        Bitmap(m_local_bitmap).set_offset(m_refresh_region.point()));
    }

    clear_refresh_drawing_pending();
  }
}

const sgfx::Theme *Component::theme() const { return event_loop()->theme(); }

const Display *Component::display() const { return event_loop()->display(); }

Display *Component::display() { return event_loop()->display(); }

void Component::erase() {
  if (is_ready_to_draw()) {
    Region window_region = Region(
      Point(m_reference_drawing_attributes.calculate_point_on_bitmap())
        + m_refresh_region.point(),
      m_refresh_region.area());

    theme()->set_display_palette(*display(), m_theme_style, m_theme_state);

#if 0
    printer::Printer p;
    p.object("erase " + name(), window_region);
#endif
    if ((window_region.width() * window_region.height()) > 0) {
      display()->set_window(window_region);
      display()->clear();
    }
  }
}

void Component::apply_antialias_filter(const DrawingAttributes &attributes) {
  if (is_ready_to_draw()) {
    if (is_antialias()) {
#if 0
			attributes.bitmap().apply_antialias_filter(
						theme().antialias_filter(),
						attributes.bitmap().region()
						);
#endif
    }
    set_refresh_drawing_pending();
  }
}

void Component::apply_antialias_filter(
  const DrawingScaledAttributes &attributes) {
  if (is_ready_to_draw()) {
#if 0
		attributes.bitmap().apply_antialias_filter(
					theme().antialias_filter(),
					attributes.bitmap().region()
					);
#endif
  }
}

const var::StringView Component::get_model() const {
  return event_loop()->model().get(name());
}

void Component::trigger_event(u32 event_type, u32 event_id) {
  event_loop()->trigger_event(
    Event().set_type(event_type).set_id(event_id).set_context(this));
}

void Component::set_model(const var::StringView value) {
  event_loop()->model().set(name(), value);
}

void Component::set_model_bool(bool value) {
  set_model(value ? "true" : "false");
}
