// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#include "ux/Model.hpp"
#include "ux/Component.hpp"

printer::Printer &
printer::operator<<(printer::Printer &printer, const ux::Model &a) {
  size_t offset = 0;
  bool is_null;
  do {
    const var::Array<var::StringView, 2> entry = a.at(offset);
    is_null = entry.at(0).is_null() || entry.at(1).is_null();
    if (!is_null) {
      printer.key(entry.at(0), entry.at(1));
    }
  } while (!is_null);
  return printer;
}

using namespace ux;

Model::Model() {}
