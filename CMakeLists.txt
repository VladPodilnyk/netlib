cmake_minimum_required (VERSION 3.1)

project (netlib)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

include_directories (
    ${PROJECT_SOURCE_DIR}/include
)

add_library ( 
    net SHARED
    ${PROJECT_SOURCE_DIR}/src/tcpserver.cpp
    ${PROJECT_SOURCE_DIR}/src/tcpclient.cpp
)

add_subdirectory (tests)
