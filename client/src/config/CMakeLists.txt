project(${CMAKE_PROJECT_NAME}_config)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(tomlplusplus CONFIG REQUIRED)
find_package(magic_enum CONFIG REQUIRED)


add_library(${PROJECT_NAME} STATIC)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

target_link_libraries(
    ${PROJECT_NAME} 
    PUBLIC 
    tomlplusplus::tomlplusplus
    ${CMAKE_PROJECT_NAME}_client_constants
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    magic_enum::magic_enum
)

target_sources(${PROJECT_NAME} PRIVATE config.cpp)

add_dependencies(${PROJECT_NAME} ${CMAKE_PROJECT_NAME}_client_constants)
