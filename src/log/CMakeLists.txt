include(AddSpdlog)

add_library(support-log
    OBJECT
        SpdLogger.h
        SpdLogger.cpp
)
add_library(support::log ALIAS support-log)

target_link_libraries(support-log PUBLIC fmt::fmt spdlog::spdlog)
target_include_directories(support-log PUBLIC ${SUPPORT_INCLUDE_DIR})
