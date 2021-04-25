project(${CMAKE_PROJECT_NAME}_client VERSION "0.5.0")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

configure_file(client_config.h.in ${PROJECT_SOURCE_DIR}/client_config.h)

find_package(Qt5 COMPONENTS Widgets Core DBus Quick Charts REQUIRED)
find_package(Qt5QuickCompiler)

set(mlfc_headers
    cpu.h
    gpu.h
    client.h
    fanmodemodel.h
    model/model.h
    model/qmltempsfanspeeds.h
)

set(mlfc_sources
    main.cpp
    cpu.cpp
    gpu.cpp
    client.cpp
    fanmodemodel.cpp
    model/model.cpp
    model/qmltempsfanspeeds.cpp
)

# #====================DBus==================================
# include_directories(${CMAKE_CURRENT_BINARY_DIR})

# qt5_add_dbus_interface(dbus_source
#     ${CMAKE_SOURCE_DIR}/dbus/com.github.mlfc.server.xml
#     interface)
# #set_source_files_properties(${dbus_source} INCLUDE ${CMAKE_SOURCE_DIR}/server/src/utils/utils.h)
# #set_source_file_property(${dbus_source} PROPERTIES INCLUDE "${CMAKE_SOURCE_DIR}/server/src/core/constants.h")
# #====================DBus==================================


# #======================Quick===============================
qtquick_compiler_add_resources(qml_resources
    ui/qml.qrc
)
# #======================Quick===============================

add_executable(${PROJECT_NAME} ${mlfc_sources} ${mlfc_headers} ${qml_resources})

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    Qt5::Widgets
    Qt5::Core
    Qt5::DBus
    Qt5::Quick
    Qt5::Charts

    ${CMAKE_PROJECT_NAME}_client_constants
    ${CMAKE_PROJECT_NAME}_server_interface
    ${CMAKE_PROJECT_NAME}_config
)

