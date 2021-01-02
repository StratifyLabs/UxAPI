// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#ifndef UXAPI_UX_EVENT_HPP_
#define UXAPI_UX_EVENT_HPP_

#include <api/api.hpp>

namespace ux {

class Component;

class Event {
public:
  Event();

  bool operator==(const Event &event) const {
    return (event.type() == type() && event.id() == id());
  }

  bool operator!=(const Event &event) const { return !(*this == event); }

  Component *component() const {
    return reinterpret_cast<Component *>(m_context);
  }

  template <class T> T *is_trigger() const {
    if (type() == T::event_type()) {
      return reinterpret_cast<T *>(context());
    }
    return nullptr;
  }
  template <class T> T *is_trigger(u32 id) const {
    if ((type() == T::event_type()) && (this->id() == id)) {
      return reinterpret_cast<T *>(context());
    }
    return nullptr;
  }

  template <class T> static Event get_event(u32 id, T *context) {
    return Event().set_type(T::event_type()).set_id(id).set_context(context);
  }

private:
  API_AF(Event, u32, type, 0);
  API_AF(Event, u32, id, 0);
  API_AF(Event, void *, context, nullptr);
};

#define EVENT_DECLARE_TYPE()                                                   \
  static u32 event_type() { return reinterpret_cast<size_t>(event_type); }

class SystemEvent : public Event {
public:
  EVENT_DECLARE_TYPE()

  SystemEvent() {
    set_type(event_type());
    set_id(event_id_none);
  }
  SystemEvent(u32 id) {
    set_type(event_type());
    set_id(id);
  }
  enum event_ids {
    event_id_none,
    event_id_enter,
    event_id_exit,
    event_id_periodic,
    event_id_transition
  };
};

} // namespace ux

#endif /* UXAPI_UX_EVENT_HPP_ */
