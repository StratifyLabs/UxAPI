
set(LIBRARIES JsonAPI FsAPI ChronoAPI VarAPI)
if(CMSDK_IS_LINK)
	list(APPEND LIBRARIES StratifyOS)
endif()

api2_add_library(
	NAME UxAPI
	DEPENDENCIES ${LIBRARIES}
	TARGETS TARGET_LIST)

foreach(TARGET ${TARGET_LIST})
	target_include_directories(${TARGET}
		PUBLIC
		$<INSTALL_INTERFACE:include/sgfx>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../sgfx/include>
		)

	target_include_directories(${TARGET}
		PUBLIC
		$<INSTALL_INTERFACE:include/sgfx>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../sgfx/include>
		)
endforeach()


