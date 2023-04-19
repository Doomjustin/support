cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

project(support
	VERSION 1.0
	LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
    # or Release
endif()

# 二进制可执行文件输出目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# 库文件（静态库，动态库）输出目录
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# 按需添加第三方库
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(AddFmt)
include(AddSpdlog)
# include(AddBoost)

# 按需
add_subdirectory(${PROJECT_SOURCE_DIR}/app)
add_subdirectory(${PROJECT_SOURCE_DIR}/src)
add_subdirectory(${PROJECT_SOURCE_DIR}/examples)

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    add_subdirectory(${PROJECT_SOURCE_DIR}/test)
endif()