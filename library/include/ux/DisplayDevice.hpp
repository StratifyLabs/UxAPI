/*! \file */ // Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
             // LICENSE.md for rights.

#ifndef UXAPI_UX_DISPLAYDEVICE_HPP_
#define UXAPI_UX_DISPLAYDEVICE_HPP_

#include <fs/File.hpp>

#include "Display.hpp"

namespace ux {

class DisplayDevice : public fs::FileAccess<DisplayFile> {
public:
  DisplayDevice();
  DisplayDevice::DisplayDevice(const var::StringView  path);

protected:
  fs::File m_device;
};

} // namespace ux

#endif /* UXAPI_UX_DISPLAYDEVICE_HPP_ */
