include (ExternalProject)
ExternalProject_Add (
    benchmark
    GIT_REPOSITORY "https://github.com/google/benchmark.git"
    GIT_TAG "v1.3.0"
    CMAKE_ARGS "-DBENCHMARK_ENABLE_TESTING=OFF;-DCMAKE_BUILD_TYPE=Release"
    SOURCE_DIR "${PROJECT_BINARY_DIR}/benchmark"
    BINARY_DIR "${PROJECT_BINARY_DIR}/benchmark"
    INSTALL_COMMAND ""
    TEST_COMMAND ""
)

set_target_properties (benchmark PROPERTIES EXCLUDE_FROM_ALL TRUE)
set (GBENCHMARK_INCLUDE_DIR "${PROJECT_BINARY_DIR}/benchmark/include")
set (GBENCHMARK_LIBRARY "${PROJECT_BINARY_DIR}/benchmark/src/libbenchmark.a")
set (
    NETLIB_BENCHMARK_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/basic_test.cpp
    ${PROJECT_SOURCE_DIR}/benchmarks/src/threadpool.cpp
    ${PROJECT_SOURCE_DIR}/src/threadpool.cpp
    ${PROJECT_SOURCE_DIR}/src/pqueue.cpp
)

add_executable (
    threadpoolbenchmark EXCLUDE_FROM_ALL 
    ${NETLIB_BENCHMARK_SOURCES}
)

include_directories ( 
    ${GBENCHMARK_INCLUDE_DIR} 
    ${PROJECT_SOURCE_DIR}/benchmarks/src/
)

target_link_libraries (
    threadpoolbenchmark
    ${GBENCHMARK_LIBRARY} 
    ${OpenCV_LIBS} 
    pthread
)

add_dependencies (threadpoolbenchmark benchmark)

