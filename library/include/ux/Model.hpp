#ifndef UXAPI_UX_MODEL_HPP
#define UXAPI_UX_MODEL_HPP

#include <printer/Printer.hpp>

#include "Event.hpp"
#include "var/StackString.hpp"

namespace ux {

class Model {
public:
  Model();

  class Entry {
    API_AC(Entry, var::KeyString, name);
    API_AC(Entry, var::String, value);

  public:
    Entry() {}

    Entry &set_value(bool value) {
      if (value) {
        set_value(var::String("true"));
      } else {
        set_value(var::String("false"));
      }
      return *this;
    }

    bool operator==(const Entry &a) const;
  };

  Model &set(const var::StringView key, const var::StringView value) {
    interface_set(key, value);
    return *this;
  }

  Model &set_bool(const var::StringView key, bool value) {
    interface_set(key, value ? "true" : "false");
    return *this;
  }

  var::StringView get(const var::StringView key) const {
    return interface_get(key);
  }

  var::Array<var::StringView, 2> at(size_t offset) const {
    return interface_at(offset);
  }

  using EntryList = var::Vector<Entry>;

protected:
  virtual void
  interface_set(const var::StringView key, const var::StringView value)
    = 0;

  virtual var::StringView interface_get(const var::StringView key) const = 0;

  virtual const var::Array<var::StringView, 2>
  interface_at(size_t offset) const = 0;
};

} // namespace ux

namespace printer {
Printer &operator<<(Printer &printer, const ux::Model &a);
}

#endif // UXAPI_UX_MODEL_HPP
