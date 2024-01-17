cmake_minimum_required(VERSION 3.12)

project(triangulate)

# Find SFML
find_package(SFML COMPONENTS graphics REQUIRED)

# Add your source files
set(SOURCES
    main.cpp
    calculate.hpp
    triangulate.hpp
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Set C++ standard to C++20
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# Link SFML
target_link_libraries(${PROJECT_NAME} sfml-graphics)
