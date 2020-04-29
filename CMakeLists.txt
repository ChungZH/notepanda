# CMakeLists.txt
cmake_minimum_required(VERSION 3.1.0)

project(notepanda)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(notepanda
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
    src/texteditor.cpp
    src/texteditor.h
    notepanda.rc
)

set(CMAKE_BUILD_TYPE Release)

target_link_libraries(notepanda Qt5::Widgets)