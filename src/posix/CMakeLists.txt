add_library(support-posix
    OBJECT
        Selector.h
        Selector.cpp
)
add_library(support::posix ALIAS support-posix)

target_link_libraries(support-posix PRIVATE support-log)
target_include_directories(support-posix PUBLIC ${SUPPORT_INCLUDE_DIR})
