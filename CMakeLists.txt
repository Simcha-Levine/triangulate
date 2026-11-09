cmake_minimum_required(VERSION 3.12)

project(triangulate)

# Find SFML
find_package(SFML COMPONENTS graphics REQUIRED)

# Specify the path to SFML static libraries
set(SFML_STATIC_LIBS_PATH ${CMAKE_SOURCE_DIR}/sfml_static)

# Add your source files
set(SOURCES
    main.cpp
    calculate.hpp
    triangulate.hpp
)

# Specify the path to SFML static libraries
# link_directories(${CMAKE_SOURCE_DIR}/sfml_static)

# Include directories for SFML headers
# target_include_directories(${PROJECT_NAME} PRIVATE ${SFML_INCLUDE_DIR})

# Link directories for SFML static libraries
# target_link_directories(${PROJECT_NAME} PRIVATE ${SFML_STATIC_LIBS_PATH})


# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Set C++ standard to C++20
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# Link SFML static libraries
target_link_libraries(${PROJECT_NAME}
    ${SFML_STATIC_LIBS_PATH}/libsfml-graphics-s.a)

# Link SFML
# target_link_libraries(${PROJECT_NAME} sfml-graphics)
# target_link_libraries(${PROJECT_NAME} sfml-graphics)

