cmake_minimum_required(VERSION 3.30)
project(mathd VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable Qt
# find_package(Qt5 REQUIRED COMPONENTS Widgets)
# set(CMAKE_AUTOUIC ON)
# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/Core/*.cpp ${PROJECT_SOURCE_DIR}/src/UI/*.cpp ${PROJECT_SOURCE_DIR}/src/*.cpp)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

set(BIN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR})

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${CURSES_LIBRARIES})

# Optional: compiler warnings
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -O2)
install(TARGETS mathd DESTINATION ${CMAKE_INSTALL_BINDIR})
