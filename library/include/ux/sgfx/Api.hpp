#ifndef UXAPI_UX_SGFX_API_HPP
#define UXAPI_UX_SGFX_API_HPP

#include <api/api.hpp>

#include "sgfx.h"

namespace ux::sgfx {
typedef api::Api<sg_api_t, SGFX_API_REQUEST> SgfxApi;

class Api : public api::ExecutionContext {
public:
  static SgfxApi &api() { return m_api; }

private:
  static SgfxApi m_api;
};
} // namespace ux::sgfx

#endif // UXAPI_UX_SGFX_API_HPP
