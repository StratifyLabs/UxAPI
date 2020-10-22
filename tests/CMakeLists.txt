

set(DEPENDENCIES TestAPI FsAPI JsonAPI)

stratify_api_add_test_executable(${PROJECT_NAME} 32768 "${DEPENDENCIES}")



