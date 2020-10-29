// Copyright 2011-2020 Tyler Gilbert and Stratify Labs, Inc; see
// LICENSE.md for rights.

#include "ux/DisplayDevice.hpp"

using namespace ux;

DisplayDevice::DisplayDevice() {}

DisplayDevice::DisplayDevice(const var::StringView path)
  : m_device(path, fs::OpenMode::read_write()) {}
