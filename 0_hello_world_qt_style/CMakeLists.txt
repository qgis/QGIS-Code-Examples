cmake_minimum_required(VERSION 3.0)

PROJECT(timtut0)
SET(CMAKE_COLOR_MAKEFILE ON)

# For mac - hard code path to Qt5 - normally done using
# -DCMAKE_PREFIX_PATH
SET(CMAKE_PREFIX_PATH /usr/local/opt/qt5 /usr/local/opt/qt5-webkit)
# set path to additional CMake modules
SET(CMAKE_MODULE_PATH
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake_find_rules
        ${CMAKE_MODULE_PATH})

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Widgets finds its own dependencies.
find_package(Qt5Widgets REQUIRED)

add_executable(hello_world main.cpp)

qt5_use_modules(hello_world Widgets)


