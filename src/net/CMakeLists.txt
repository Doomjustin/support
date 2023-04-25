add_library(support-net
    OBJECT
        Socket.cpp
        Connection.cpp
        Connector.cpp
        Acceptor.cpp
)
add_library(support::net ALIAS support-net)

target_include_directories(support-net PUBLIC ${SUPPORT_INCLUDE_DIR})

target_link_libraries(support-net
    PRIVATE
        support::log
        support::posix
)