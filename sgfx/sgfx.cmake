
if(NOT DEFINED API_IS_SDK)
	include(API)
	sos_sdk_include_target(sgfx "${API_CONFIG_LIST}")
	sos_sdk_include_target(sgfx_1bpp "${API_CONFIG_LIST}")
	sos_sdk_include_target(sgfx_2bpp "${API_CONFIG_LIST}")
	sos_sdk_include_target(sgfx_4bpp "${API_CONFIG_LIST}")
	sos_sdk_include_target(sgfx_8bpp "${API_CONFIG_LIST}")
	sos_sdk_include_target(sgfx_16bpp "${API_CONFIG_LIST}")
endif()
