cmake_minimum_required(VERSION 3.10)

project(mlfc_client)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core DBus Quick REQUIRED)
find_package(Qt5QuickCompiler)

set(mlfc_headers
    src/cpu.h
    src/gpu.h
    src/client.h
    src/serverstates.h
)

set(mlfc_sources
    src/main.cpp
    src/cpu.cpp
    src/gpu.cpp
    src/client.cpp
)

#====================DBus==================================
include_directories(${CMAKE_CURRENT_BINARY_DIR})

qt5_add_dbus_interface(dbus_source
    ${CMAKE_SOURCE_DIR}/dbus/com.github.mlfc.server.xml
    interface)
#====================DBus==================================


#======================Quick===============================
qtquick_compiler_add_resources(qml_resources
    src/ui/qml.qrc
)
#======================Quick===============================

add_executable(${PROJECT_NAME} ${mlfc_sources} ${mlfc_headers} ${dbus_source} ${qml_resources})
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::DBus Qt5::Quick)

