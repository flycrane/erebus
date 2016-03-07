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

include_directories(src)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Werror")

add_subdirectory(src)

add_executable(erebus ${SOURCE})

target_link_libraries(erebus dl)
