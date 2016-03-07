add_executable(utils__file_utils__path_test
        ${CMAKE_CURRENT_SOURCE_DIR}/path_test.cc
        ${HEADER}
        ${SOURCE}
)
add_dependencies(utils__file_utils__path_test googletest)
target_link_libraries(utils__file_utils__path_test ${TEST_LIBRARIES})
add_test(utils__file_utis__path_test utils__file_utils__path_test)

add_executable(utils__file_utils__file_test
        ${CMAKE_CURRENT_SOURCE_DIR}/file_test.cc
        ${HEADER}
        ${SOURCE}
)
add_dependencies(utils__file_utils__file_test googletest)
target_link_libraries(utils__file_utils__file_test ${TEST_LIBRARIES})
add_test(utils__file_utis__file_test utils__file_utils__file_test)

add_executable(utils__file_utils__directory_test
        ${CMAKE_CURRENT_SOURCE_DIR}/directory_test.cc
        ${HEADER}
        ${SOURCE}
)
add_dependencies(utils__file_utils__directory_test googletest)
target_link_libraries(utils__file_utils__directory_test ${TEST_LIBRARIES})
add_test(utils__file_utis__directory_test utils__file_utils__directory_test)
