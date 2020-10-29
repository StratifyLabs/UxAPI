
if(NOT DEFINED IS_SDK)
	include(JsonAPI)
	include(HalAPI)
	sos_sdk_include_target(sgfx "${STRATIFYAPI_CONFIG_LIST}")
	sos_sdk_include_target(UxAPI "${STRATIFYAPI_CONFIG_LIST}")
endif()
