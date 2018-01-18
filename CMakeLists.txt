cmake_minimum_required (VERSION 3.1)

project (netlib)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set (
    NETLIB_SRC_FILES
    ${PROJECT_SOURCE_DIR}/src/tcpserver.cpp
    ${PROJECT_SOURCE_DIR}/src/tcpclient.cpp
    ${PROJECT_SOURCE_DIR}/src/threadpool.cpp
    ${PROJECT_SOURCE_DIR}/src/pqueue.cpp
)

include_directories (
    ${PROJECT_SOURCE_DIR}/include
)

add_library ( 
    net SHARED
    ${NETLIB_SRC_FILES}
)

target_link_libraries (
    net
    pthread
)

add_subdirectory (tests)
add_subdirectory (examples)
add_subdirectory (benchmarks)