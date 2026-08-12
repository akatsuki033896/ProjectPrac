cmake_minimum_required(VERSION 3.10)

project(test_pugi_xml LANGUAGES CXX)

# Use a stable C++ standard; pugixml 1.14 only needs C++11.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Keep the executable and its data file in one place so the relative
# load_file("test.xml") call in main.cpp resolves at runtime.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# pugixml is a single-translation-unit library: compile its .cpp directly.
add_executable(test_pugi_xml
    main.cpp
    pugixml/pugixml.cpp
)

# main.cpp uses  #include "pugixml/pugixml.hpp"   -> needs project root
# pugixml.cpp uses #include "pugixml.hpp"          -> needs the pugixml/ subdir
target_include_directories(test_pugi_xml PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/pugixml
)