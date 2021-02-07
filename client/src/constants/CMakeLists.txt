project(${CMAKE_PROJECT_NAME}_client_constants)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core)

add_library(${PROJECT_NAME} STATIC)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Core)

target_sources(
    ${PROJECT_NAME}
    PRIVATE
    constants.h
    enumerationstorage.h
)
