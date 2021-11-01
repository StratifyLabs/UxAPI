cmake_minimum_required (VERSION 3.10)

project(UxAPI CXX C)
include(CTest)

add_subdirectory(sgfx sgfx)
add_subdirectory(library library)
if(SOS_IS_TEST)
	add_subdirectory(tests tests)
endif()
