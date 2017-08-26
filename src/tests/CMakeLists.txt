# executable for the client tests
add_executable (client_test.exec client_test.cc)
target_link_libraries (client_test.exec m ssl crypto libclient libpuzzle)
set_target_properties (client_test.exec PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# executable for the server tests
add_executable (server_test.exec server_test.cc)
target_link_libraries (server_test.exec libserver m ssl crypto libclient libpuzzle)
set_target_properties (server_test.exec PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# executable for the opt server tests
add_executable (optserver_test.exec optserver_test.cc)
target_link_libraries (optserver_test.exec libserver m ssl crypto libclient libpuzzle)
set_target_properties (optserver_test.exec PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
