cmake_minimum_required(VERSION 3.19)
project(expressionRepl LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find all .cpp files in src/, EXCEPT main.cpp
file(GLOB SRC_FILES src/*.cpp)
list(REMOVE_ITEM SRC_FILES ${CMAKE_SOURCE_DIR}/src/main.cpp)  # Exclude main.cpp

# Create a library from the source files
add_library(expressionReplLib ${SRC_FILES})

# Include directories for headers
target_include_directories(expressionReplLib PUBLIC include)

# Enable warnings
target_compile_options(expressionReplLib PRIVATE -Wall -std=c++20 -fpermissive)

# Create the main executable and link the library
add_executable(expressionRepl src/main.cpp)
target_link_libraries(expressionRepl PRIVATE expressionReplLib)

# Enables CTest
enable_testing()

# Create the test executable and link the same library
add_executable(taylorSeries test/taylorSeries.cpp)
target_link_libraries(taylorSeries PRIVATE expressionReplLib)
# target_include_directories(taylorSeries PRIVATE include)

# Register it with CTest
add_test(NAME taylorSeries COMMAND taylorSeries)


#test command
add_custom_target(run_tests
	COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
	DEPENDS taylorSeries  # Ensure tests are built before running
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
