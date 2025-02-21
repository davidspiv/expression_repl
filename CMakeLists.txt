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
target_compile_options(expressionRepl PRIVATE -c -g -Wall -std=c++17 -fpermissive)

# Set the output directory to the top-level project folder
set_target_properties(expressionRepl PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

# Enables CTest
enable_testing()

# Add a test executable
add_executable(Tests test/main.cpp)

# Register it with CTest
add_test(NAME Tests COMMAND Tests)

# Custom target to run tests automatically after building
# add_custom_target(run_tests ALL
#     COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
#     DEPENDS Tests
#     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
# )

# add_dependencies(Tests run_tests)
