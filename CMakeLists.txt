cmake_minimum_required(VERSION 3.19)
project(expressionRepl LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find all .cpp files in src/
file(GLOB SOURCES src/*.cpp)

# Create an executable
add_executable(expressionRepl ${SOURCES})

# Include directories for headers
target_include_directories(expressionRepl PRIVATE include)

# Enable warnings
target_compile_options(expressionRepl PRIVATE -Wall -Wextra -Wpedantic)

# Set the output directory to the top-level project folder
set_target_properties(expressionRepl PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)
