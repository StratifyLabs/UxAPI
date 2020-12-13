
set(LIBRARIES	JsonAPI FsAPI ChronoAPI VarAPI)
if(SOS_IS_LINK)
	list(APPEND LIBRARIES StratifyOS)
endif()

api_add_api_library(${PROJECT_NAME} "${LIBRARIES}")


foreach(ARCH ${SOS_ARCH_LIST})
	target_include_directories(UxAPI_release_${ARCH}
		PUBLIC
		$<INSTALL_INTERFACE:include/sgfx>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../sgfx/include>
		)

	target_include_directories(UxAPI_debug_${ARCH}
		PUBLIC
		$<INSTALL_INTERFACE:include/sgfx>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../sgfx/include>
		)
endforeach()

target_include_directories(UxAPI_release_${SOS_ARCH}
	PUBLIC
	$<INSTALL_INTERFACE:include/sgfx>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../sgfx/include>
	)

target_include_directories(UxAPI_debug_${SOS_ARCH}
	PUBLIC
	$<INSTALL_INTERFACE:include/sgfx>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../sgfx/include>
	)

if(SOS_IS_LINK)
	target_include_directories(UxAPI_coverage_${SOS_ARCH}
		PUBLIC
		$<INSTALL_INTERFACE:include/sgfx>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../sgfx/include>
		)
endif()


