# 按需添加第三方库
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(AddFmt)

set(SUPPORT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(SUPPORT_INTERNAL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/support)

# Log Module
add_subdirectory(Log)


add_library(support
    STATIC
        Log.cpp
        ThreadPool.cpp
)

target_include_directories(support PUBLIC ${SUPPORT_INCLUDE_DIR})

target_link_libraries(support PUBLIC fmt::fmt)
target_link_libraries(support
    PRIVATE
        support-log
)
