cmake_minimum_required(VERSION 3.10)

# Project name and version
project(smctl VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Specify the include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Collect all source files in src directory
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
# Define your binary output dir
set(BIN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin")

# Optional: make sure the binary is built into bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR})
# Add executable target
add_executable(${PROJECT_NAME} ${SOURCES})
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin)
# Optionally, specify output directory for binaries
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/bin
)
# install(TARGAETS clicalc RUNTIME DESTINATION bin)
# Optionally, add compile options (uncomment if needed)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
add_custom_target(run
    COMMAND ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
    COMMENT "Running smctl"
)
# Optionally, add link libraries (uncomment and modify if needed)
# target_link_libraries(${PROJECT_NAME} PRIVATE some_library)
#find_package(fmt REQUIRED)
#target_link_libraries(smctl PRIVATE fmt::fmt)
