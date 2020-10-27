
if(NOT DEFINED IS_SDK)
	include(JsonAPI)
	sos_sdk_include_target(UxAPI "${STRATIFYAPI_CONFIG_LIST}")
endif()
