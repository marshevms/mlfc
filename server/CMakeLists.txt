cmake_minimum_required(VERSION 3.10)

project(mlfc_server VERSION "0.5.1")

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


configure_file(server_info.h.in ${PROJECT_SOURCE_DIR}/server_info.h)

find_package(Qt5 COMPONENTS Core DBus REQUIRED)

set(mlfc_headers
    src/server.h
    src/core/constants.h
    src/core/reader.h
    src/serveradapter.h
    src/utils/utils.h
    src/core/manager.h
)

set(mlfc_sources
    src/main.cpp
    src/server.cpp
    src/core/constants.cpp
    src/core/reader.cpp
    src/serveradapter.cpp
    src/utils/utils.cpp
    src/core/manager.cpp
)

#Dbus
#include_directories(${CMAKE_CURRENT_BINARY_DIR})

#qt5_generate_dbus_interface(
#    src/serveradapter.h
#    com.github.mlfc.server.xml
#    OPTIONS -A
#)

#qt5_add_dbus_adaptor(dbus_sources
#    ${CMAKE_CURRENT_BINARY_DIR}/com.github.mlfc.server.xml
#    ${CMAKE_CURRENT_SOURCE_DIR}/src/server.h
#    mlfc::Server
#    serveradapter
#    )


add_executable(${PROJECT_NAME} ${mlfc_sources} ${mlfc_headers})
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::DBus)
