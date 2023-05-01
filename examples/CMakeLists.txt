add_executable(simple-client simple_client.cpp)
target_link_libraries(simple-client PRIVATE support)

add_executable(simple-server simple_server.cpp)
target_link_libraries(simple-server PRIVATE support)

add_executable(multi-thread-server multi_thread_server.cpp)
target_link_libraries(multi-thread-server PRIVATE support)

add_executable(echo-server echo_server.cpp)
target_link_libraries(echo-server PRIVATE support)