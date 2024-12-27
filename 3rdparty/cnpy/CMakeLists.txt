CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

project(CNPY)
set(CMAKE_CXX_STANDARD 11)

option(ENABLE_STATIC "Build static (.a) library" ON)

find_package(ZLIB REQUIRED)

include_directories(${ZLIB_INCLUDE_DIRS})

add_library(cnpy SHARED "cnpy.cpp")
target_include_directories(cnpy PUBLIC "./")
target_link_libraries(cnpy ${ZLIB_LIBRARIES})

