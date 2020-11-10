
set(LIBRARIES	JsonAPI FsAPI ChronoAPI VarAPI)
if(SOS_IS_LINK)
	list(APPEND LIBRARIES StratifyOS)
endif()


stratifyapi_add_api_library(${PROJECT_NAME} "${LIBRARIES}")



include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../sgfx/include)

