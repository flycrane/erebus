cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(erebus CXX)

###########################################
##Enable testing with googletest
###########################################

# Enable ExternalProject CMake module
include(ExternalProject)

# Add gtest
ExternalProject_Add(
                googletest
                SVN_REPOSITORY http://googletest.googlecode.com/svn/trunk/
                TIMEOUT 10
                CMAKE_ARGS -Dgtest_force_shared_crt=ON
                INSTALL_COMMAND ""
                LOG_DOWNLOAD ON
                LOG_CONFIGURE ON
                LOG_BUILD ON
)

# Specify include dir
ExternalProject_Get_Property(googletest source_dir)
include_directories(${source_dir}/include)


# Specify MainTest's link libraries
ExternalProject_Get_Property(googletest binary_dir)
if(MSVC)
    set(Suffix ".lib")
else()
    set(Suffix ".a")
    set(Pthread "-pthread")
endif()

set(gtest_LIBRARIES 
        ${binary_dir}/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${Suffix}
        ${binary_dir}/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${Suffix}
        ${Pthread}
)

enable_testing()

##############################################
##End of googletest
##############################################

#provide boost headers
add_definitions(-DBOOST_LOG_DYN_LINK)
find_package(Boost COMPONENTS log log_setup thread date_time filesystem system REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

include_directories(src)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Werror")

set(TEST_LIBRARIES ${gtest_LIBRARIES} dl pthread ${Boost_LIBRARIES})

add_subdirectory(src)
add_subdirectory(test)

add_executable(erebus ${SOURCE} ${HEADER} src/main.cc)

target_link_libraries(erebus dl)
target_link_libraries(erebus ${Boost_LIBRARIES} pthread)
