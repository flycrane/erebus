add_executable(utils__file_utils__path_test
        ${CMAKE_CURRENT_SOURCE_DIR}/path_test.cc
        ${HEADER}
        ${SOURCE}
)
add_dependencies(utils__file_utils__path_test googletest)
target_link_libraries(utils__file_utils__path_test ${TEST_LIBRARIES})
add_test(utils__file_utis__path_test utils__file_utils__path_test)
