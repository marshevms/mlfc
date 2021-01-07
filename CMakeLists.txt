cmake_minimum_required(VERSION 3.10)

project(mlfc)

enable_testing()
add_test(NAME ${CMAKE_PROJECT_NAME}_config_test
        COMMAND ${CMAKE_PROJECT_NAME}_config_test)

add_subdirectory(client)
add_subdirectory(server)
