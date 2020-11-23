

set(DEPENDENCIES TestAPI FsAPI JsonAPI)

api_add_test_executable(${PROJECT_NAME} 32768 "${DEPENDENCIES}")



