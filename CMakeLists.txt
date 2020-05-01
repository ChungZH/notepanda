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

if(WIN32)
    set(GUI_TYPE WIN32)
endif()

set(SOURCES
    src/main.cpp
    src/ui/mainwindow.cpp
    src/ui/mainwindow.h
    src/ui/linenumberarea.h
    src/ui/mainwindow.ui
    src/core/texteditor.cpp
    src/core/texteditor.h
    notepanda.rc
    resources.qrc
)

add_executable(notepanda
    ${GUI_TYPE}
    ${SOURCES}
)

set(CMAKE_BUILD_TYPE Release)

target_link_libraries(notepanda Qt5::Widgets)