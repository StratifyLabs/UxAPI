
if(NOT DEFINED API_IS_SDK)
	include(JsonAPI)
	include(sgfx)
	sos_sdk_include_target(UxAPI "${API_CONFIG_LIST}")
endif()
