project(${CMAKE_PROJECT_NAME}_config_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(GTest CONFIG REQUIRED)

add_executable(${PROJECT_NAME})

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    GTest::gtest
    GTest::gtest_main
    
    ${CMAKE_PROJECT_NAME}_config)

target_sources(
    ${PROJECT_NAME} 
    PRIVATE
    config.cpp
)

add_dependencies(${PROJECT_NAME} ${CMAKE_PROJECT_NAME}_config)
