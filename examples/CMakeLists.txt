add_executable (
    server EXCLUDE_FROM_ALL
    ${PROJECT_SOURCE_DIR}/examples/server.cpp
    ${NETLIB_SRC_FILES}
)

add_executable (
    client EXCLUDE_FROM_ALL
    ${PROJECT_SOURCE_DIR}/examples/client.cpp
)

add_dependencies (server net)
add_dependencies (client net)

target_link_libraries (
    server
    net
)

target_link_libraries (
    client
    net
)