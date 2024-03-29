// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#ifndef UXAPI_UX_LAYOUT_HPP
#define UXAPI_UX_LAYOUT_HPP

#include <functional>

#include "Component.hpp"
#include "TouchGesture.hpp"

namespace ux {

class Layout : public ComponentAccess<Layout> {
public:
  enum class IsRecursive { no, yes };

  class Item {
  public:
    Item(Component *component) {
      m_component = component;
      m_drawing_point = component->reference_drawing_attributes().point();
      m_drawing_area = component->reference_drawing_attributes().area();
    }

    Component *component() const { return m_component; }

    const DrawingPoint &drawing_point() const { return m_drawing_point; }

    const DrawingArea &drawing_area() const { return m_drawing_area; }

    void set_drawing_point(const DrawingPoint &point) {
      m_drawing_point = point;
    }

    void set_drawing_area(const DrawingArea &area) { m_drawing_area = area; }

    void set_component(Component *component) { m_component = component; }

  private:
    Component *m_component = nullptr;
    DrawingPoint m_drawing_point;
    DrawingArea m_drawing_area;
  };

  enum class Flow { vertical, horizontal, free, vertical_no_scroll };

  Layout(const var::StringView name);

  virtual ~Layout();

  Layout &add_component(Component &component);
  Layout &delete_component(const var::StringView component_name);

  Layout &set_vertical_scroll_enabled(bool value = true) {
    set_vertical_scroll_enabled_internal(true);
    return *this;
  }

  Layout &set_horizontal_scroll_enabled(bool value = true) {
    set_horizontal_scroll_enabled_internal(true);
    return *this;
  }

  void update_drawing_area(const Component *component, const DrawingArea &area);

  void
  update_drawing_area(const var::StringView name, const DrawingArea &area) {
    update_drawing_area(find<Component>(name), area);
  }

  void
  update_drawing_point(const Component *component, const DrawingPoint &point);

  void
  update_drawing_point(const var::StringView name, const DrawingPoint &point) {
    update_drawing_point(find<Component>(name), point);
  }

  bool is_owner(const Event &event) { return is_owner(event.component()); }

  bool is_owner(const Component *component) {
    if (component == nullptr) {
      return false;
    }

    for (Item &cp : m_component_list) {
      if (cp.component() == component) {
        return true;
      }
      if (cp.component() && cp.component()->is_layout()) {
        if (static_cast<Layout *>(cp.component())->is_owner(component)) {
          return true;
        }
      }
    }
    return false;
  }

  Layout *find_layout(const var::StringView name) {
    for (Item &cp : m_component_list) {
      if (
        cp.component() && (name == cp.component()->name())
        && cp.component()->is_layout()) {
        return static_cast<Layout *>(cp.component());
      }
    }
    return nullptr;
  }

  template <class T, bool is_fatal = true>
  T *search(const var::StringView name) {
    for (Item &cp : m_component_list) {
      if (cp.component() && cp.component()->is_layout()) {
        T *result
          = static_cast<Layout *>(cp.component())->search<T, false>(name);
        if (result) {
          return result;
        }
      }

      if (
        (name == cp.component()->name()) || (cp.component()->name() == name)) {
        return static_cast<T *>(cp.component());
      }
    }
    if (is_fatal) {
      printf("Failed to search %s\n", var::IdString(name).cstring());
      abort();
    }
    return nullptr;
  }

  template <class T, bool is_fatal = true> T *find(const var::StringView name) {
    for (Item &cp : m_component_list) {
      if (cp.component() && (name == cp.component()->name())) {
        return static_cast<T *>(cp.component());
      }
    }
    if (is_fatal) {
      printf(
        "%s Failed to find %s\n",
        var::IdString(this->name()).cstring(),
        var::IdString(name).cstring());
      abort();
    }
    return nullptr;
  }

  void scroll(DrawingPoint value);
  void erase_region(DrawingRegion region);

  virtual void draw(const DrawingAttributes &attributes);

  void distribute_event(const ux::Event &event);

  Layout &set_focus(bool value = true);

  void trigger_event(u32 event_type, u32 event_id) {
    Component::trigger_event(event_type, event_id);
  }

  const DrawingPoint &origin() const { return m_origin; }

protected:
private:
  friend class Controller;
  friend class EventLoop;
  API_AF(Layout, Flow, flow, Flow::free);
  API_ACCESS_COMPOUND(Layout, var::Vector<Item>, component_list);
  DrawingPoint m_origin;
  DrawingArea m_area;

  void shift_origin(DrawingPoint shift);
  drawing_int_t handle_vertical_scroll(sg_int_t scroll);
  drawing_int_t handle_horizontal_scroll(sg_int_t scroll);

  void generate_layout_positions();
  void generate_vertical_layout_positions();
  void generate_vertical_no_scroll_layout_positions();
  void generate_horizontal_layout_positions();
  void generate_free_layout_positions();

  void examine_visibility();

  void set_refresh_region(const sgfx::Region &region);
  void touch_drawing_attributes() { shift_origin(DrawingPoint(0, 0)); }
};

template <class T> class LayoutAccess : public Layout {
public:
  LayoutAccess<T>(const var::StringView name) : Layout(name) {}

  T &add_component(Component &component) {
    return static_cast<T &>(Layout::add_component(component));
  }

  T &delete_component(const var::StringView component_name) {
    return static_cast<T &>(Layout::delete_component(component_name));
  }

  API_ACCESS_DERIVED_FUNDAMETAL(Layout, T, Flow, flow)
  API_ACCESS_DERIVED_BOOL(Layout, T, vertical_scroll_enabled)
  API_ACCESS_DERIVED_BOOL(Layout, T, horizontal_scroll_enabled)

  T &set_focus(bool value = true) {
    Layout::set_focus(value);
    return static_cast<T &>(*this);
  }

  COMPONENT_ACCESS_CREATE()
  COMPONENT_ACCESS_DERIVED(Layout, T)

protected:
};

} // namespace ux

#endif // UXAPI_UX_LAYOUT_HPP
