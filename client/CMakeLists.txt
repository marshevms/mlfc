cmake_minimum_required(VERSION 3.10)

project(mlfc_client)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets Core DBus Quick REQUIRED)
find_package(Qt5QuickCompiler)

set(mlfc_headers
    src/cpu.h
    src/gpu.h
    src/client.h
    src/enumerationstorage.h
    src/fanmodemodel.h
)

set(mlfc_sources
    src/main.cpp
    src/cpu.cpp
    src/gpu.cpp
    src/client.cpp
    src/serverinterface.cpp
    src/fanmodemodel.cpp
)
include_directories(${CMAKE_SOURCE_DIR}/server/src/utils)

#====================DBus==================================
include_directories(${CMAKE_CURRENT_BINARY_DIR})

qt5_add_dbus_interface(dbus_source
    ${CMAKE_SOURCE_DIR}/dbus/com.github.mlfc.server.xml
    interface)
#set_source_files_properties(${dbus_source} INCLUDE ${CMAKE_SOURCE_DIR}/server/src/utils/utils.h)
#set_source_file_property(${dbus_source} PROPERTIES INCLUDE "${CMAKE_SOURCE_DIR}/server/src/core/constants.h")
#====================DBus==================================


#======================Quick===============================
qtquick_compiler_add_resources(qml_resources
    src/ui/qml.qrc
)
#======================Quick===============================

add_executable(${PROJECT_NAME} ${mlfc_sources} ${mlfc_headers} ${qml_resources})
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Core Qt5::DBus Qt5::Quick)

