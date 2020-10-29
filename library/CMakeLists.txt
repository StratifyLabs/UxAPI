
set(LIBRARIES ChartAPI JsonAPI FsAPI ChronoAPI VarAPI)


stratifyapi_add_api_library(${PROJECT_NAME} "${LIBRARIES}")



include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../sgfx/include)

