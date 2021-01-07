project(${CMAKE_PROJECT_NAME}_server_interface)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS Core DBus REQUIRED)

add_library(${PROJECT_NAME} STATIC)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/server/src/utils)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    Qt5::Core
    Qt5::DBus
    ${CMAKE_PROJECT_NAME}_client_constants
)

target_sources(${PROJECT_NAME} PRIVATE serverinterface.cpp)

add_dependencies(${PROJECT_NAME} ${CMAKE_PROJECT_NAME}_client_constants)