include (ExternalProject)
ExternalProject_Add (
    googletest
    GIT_REPOSITORY "https://github.com/google/googletest.git"
    UPDATE_COMMAND cd ${PROJECT_BINARY_DIR}/googletest && mv googletest gtest && mv gtest/googletest .
    CMAKE_ARGS "-DCMAKE_BUILD_TYPE=Release"
    SOURCE_DIR "${PROJECT_BINARY_DIR}/googletest/googletest"
    BINARY_DIR "${PROJECT_BINARY_DIR}/googletest/googletest"
    INSTALL_COMMAND ""
    TEST_COMMAND ""
)
set_target_properties (googletest PROPERTIES EXCLUDE_FROM_ALL TRUE)
set (GTEST_INCLUDE_DIRS "${PROJECT_BINARY_DIR}/googletest/googletest/include")
set (GTEST_LIBRARIES "${PROJECT_BINARY_DIR}/googletest/googletest/libgtest.a")
set (GTEST_MAIN_LIBRARIES "${PROJECT_BINARY_DIR}/googletest/googletest/libgtest_main.a")
set (GTEST_BOTH_LIBRARIES ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})

include_directories (
    ${GTEST_INCLUDE_DIRS}
)

add_executable ( 
    net_test EXCLUDE_FROM_ALL
    ${PROJECT_SOURCE_DIR}/tests/tcpserver_test.cpp
    ${NETLIB_SRC_FILES}
)

target_link_libraries (
    net_test
    pthread
    ${GTEST_BOTH_LIBRARIES}
)

add_dependencies (net_test googletest)
