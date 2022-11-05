set(LIBRARIES	JsonAPI FsAPI ChronoAPI VarAPI)
if(CMSDK_IS_LINK)
	list(APPEND LIBRARIES StratifyOS)
endif()

cmsdk2_check_version(
	NAME JsonAPI
	VERSION ${JsonAPI_VERSION}
	MINIMUM_VERSION 1.5.2)

api2_add_library(
	NAME ${PROJECT_NAME}
	DEPENDENCIES ${LIBRARIES}
	TARGETS TARGET_LIST)

foreach(TARGET ${TARGET_LIST})
	target_include_directories(${TARGET}
		PUBLIC
		$<INSTALL_INTERFACE:include/sgfx>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../sgfx/include>)
endforeach()


