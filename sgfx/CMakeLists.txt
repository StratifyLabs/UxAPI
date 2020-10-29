
set(LIBRARIES StratifyOS)


stratifyapi_add_api_library(sgfx "${LIBRARIES}" "")
stratifyapi_add_api_library_option(sgfx "${LIBRARIES}" 1bpp)
stratifyapi_add_api_library_option(sgfx "${LIBRARIES}" 2bpp)
stratifyapi_add_api_library_option(sgfx "${LIBRARIES}" 4bpp)
stratifyapi_add_api_library_option(sgfx "${LIBRARIES}" 16bpp)



