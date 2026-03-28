cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

project(calculator)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets REQUIRED)

file(GLOB sources *.cpp)
add_executable(calculator ${sources})
target_link_libraries(calculator PRIVATE Qt6::Core Qt6::Widgets)
target_include_directories(calculator PRIVATE ${Qt6_INCLUDE_DIRS})