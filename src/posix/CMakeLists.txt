add_library(support-posix
    OBJECT
        PosixUtils.cpp
        schedular/Selector.cpp
        net/PosixSocket.cpp
)
add_library(support::posix ALIAS support-posix)

target_link_libraries(support-posix PRIVATE support-log)
target_include_directories(support-posix PUBLIC ${SUPPORT_INCLUDE_DIR})
