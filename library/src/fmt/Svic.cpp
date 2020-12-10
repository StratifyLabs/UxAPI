/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.
#include "ux/fmt/Svic.hpp"

using namespace ux::fmt;

Svic::Svic(const fs::FileObject *file_object) {
  API_ASSERT(file_object != nullptr);

  m_current_icon_at = static_cast<u32>(-1);
  parse_icons();
}

void Svic::parse_icons() {
  int cursor = m_file_object->location();
  sg_vector_icon_header_t header;

  u32 item_count = 0;
  while (m_file_object->read(var::View(header)).return_value() > 0) {
    item_count++;
    m_file_object->seek(
      header.count * sizeof(sg_vector_path_description_t),
      fs::File::Whence::current);
  }

  m_file_object->seek(cursor, fs::File::Whence::set);
  m_icons.reserve(item_count);

  while (m_file_object->read(var::View(header)).return_value() > 0) {
    m_icons.push_back(header);
    m_file_object->seek(
      header.count * sizeof(sg_vector_path_description_t),
      fs::File::Whence::current);
  }

  m_file_object->seek(cursor, fs::File::Whence::set);
}

var::StringView Svic::name_at(u32 i) const {
  if (i < count()) {
    return var::StringView(m_icons.at(i).name);
  }
  return var::StringView();
}

ux::sgfx::VectorPath Svic::get(const var::StringView &name) const {
  for (u32 i = 0; i < count(); i++) {
    if (name == m_icons.at(i).name) {
      return at(i);
    }
  }
  return sgfx::VectorPath();
}

ux::sgfx::VectorPath Svic::at(u32 i) const {
  sgfx::VectorPath vector_path;
  if (m_current_icon_at != i) {
    var::View(m_current_icon.resize(m_icons.at(i).count)).fill(0);
    m_file_object->seek(m_icons.at(i).list_offset).read(m_current_icon);
    m_current_icon_at = i;
  }
  vector_path << m_current_icon;
  return vector_path;
}

Svic &Svic::append(
  const var::StringView name,
  const var::Vector<sg_vector_path_description_t> &list) {

  sg_vector_icon_header_t header = {0};
  var::View(header).pop_back().copy(name);
  header.count = list.count();
  header.list_offset = m_file_object->location() + sizeof(header);
  m_file_object->write(var::View(header)).write(list);
  return *this;
}
