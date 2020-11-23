
if(NOT DEFINED IS_SDK)
	include(JsonAPI)
	include(sgfx)
	sos_sdk_include_target(UxAPI "${API_CONFIG_LIST}")
endif()
