add_executable(simple-client simple_client.cpp)
target_link_libraries(simple-client PRIVATE support)

add_executable(simple-server simple_server.cpp)
target_link_libraries(simple-server PRIVATE support)