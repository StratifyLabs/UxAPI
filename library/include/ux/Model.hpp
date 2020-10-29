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

  void update(const Entry &entry);

  const var::StringView lookup(const var::StringView key) const;

  static var::String from_list(std::initializer_list<var::String> il) {
    var::String v;
    for (auto i : il) {
      v += i + ";";
    }
    v.pop_back();
    return v;
  }

  static var::String from_list(const var::StringList &list) {
    var::String v;
    for (auto i : list) {
      v += i + ";";
    }
    v.pop_back();
    return v;
  }

  template <typename T>
  static var::String from_list(std::initializer_list<T> il) {
    var::String v;
    for (auto i : il) {
      v += var::NumberString(i) + ";";
    }
    v.pop_back();
    return std::move(v);
  }

  static var::StringViewList to_list(const var::StringView value) {
    return value.split(";");
  }

  using EntryList = var::Vector<Entry>;

private:
  API_AC(Model, EntryList, entry_list);
};

} // namespace ux

namespace printer {
Printer &operator<<(Printer &printer, const ux::Model &a);
}

#endif // UXAPI_UX_MODEL_HPP
