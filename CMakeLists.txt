cmake_minimum_required(VERSION 3.12)
cmsdk2_minimum_required(2.1.2)
project(UxAPI
	LANGUAGES CXX C
	VERSION 1.2.0)
include(CTest)

add_subdirectory(sgfx sgfx)
add_subdirectory(library library)
option(UX_API_IS_TEST "Enable Ux API tests" OFF)
if(UX_API_IS_TEST)
	add_subdirectory(tests tests)
endif()
