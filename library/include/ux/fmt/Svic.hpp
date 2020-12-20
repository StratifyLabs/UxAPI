// Copyright 2016-2021 Tyler Gilbert and Stratify Labs, Inc; see LICENSE.md

#ifndef SAPI_FMT_SVIC_HPP_
#define SAPI_FMT_SVIC_HPP_

#include <fs/File.hpp>
#include <var/Vector.hpp>

#include "../sgfx/Vector.hpp"

namespace ux::fmt {

/*! \brief Svc (Stratify Vector Collection) Class
 * \details The Svc class manages files that
 * contain collections of Stratify Vector icons.
 *
 */
class Svic {
public:
  Svic(const fs::FileObject *file_object);

  u32 count() const { return m_icons.count(); }

  var::StringView name_at(u32 i) const;

  Svic &append(
    const var::StringView name,
    const var::Vector<sg_vector_path_description_t> &list);

  ux::sgfx::VectorPath get(const var::StringView &name) const;
  ux::sgfx::VectorPath at(u32 i) const;

private:
  var::Vector<sg_vector_icon_header_t> m_icons;

  // these track internal state used for caching
  mutable u32 m_current_icon_at;
  mutable var::Vector<sg_vector_path_description_t> m_current_icon;

  void parse_icons();

  const fs::FileObject *m_file_object;
};

} // namespace ux::fmt

#endif // SAPI_FMT_SVIC_HPP_
