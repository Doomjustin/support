function(add_catch2_test target source)
    add_executable(${target} ${source})

    target_link_libraries(${target}
        PRIVATE
            support
            Catch2::Catch2WithMain)
endfunction()


function(add_support_test target source)
    add_executable(${target} ${source})
    target_link_libraries(${target} support)
endfunction()

add_support_test(test-log test_log.cpp)
add_support_test(test-threadpool test_thread_pool.cpp)
add_support_test(test-timer test_timer.cpp)


list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(AddCatch2)
include(CTest)

add_executable(test-catch2 test_catch2.cpp)
target_link_libraries(test-catch2 PRIVATE Catch2::Catch2WithMain)
catch_discover_tests(test-catch2)

add_catch2_test(test-list test_list.cpp)