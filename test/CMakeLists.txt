list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(AddCatch2)
include(CTest)

add_executable(test-catch2 test_catch2.cpp)
target_link_libraries(test-catch2 PRIVATE Catch2::Catch2WithMain)
catch_discover_tests(test-catch2)