# CMakeLists.txt
cmake_minimum_required(VERSION 3.1.0)

file(STRINGS "${CMAKE_SOURCE_DIR}/makespec/VERSION" VERSION)
file(STRINGS "${CMAKE_SOURCE_DIR}/makespec/BUILDVERSION" BUILD_VERSION)
file(STRINGS "${CMAKE_SOURCE_DIR}/makespec/VERSIONSUFFIX" VERSION_SUFFIX)

set(VERSION_STRING "${VERSION}${VERSION_SUFFIX}")
project(notepanda)

set(VERSION_LIST ${VERSION})
string(REPLACE "." ";" VERSION_LIST ${VERSION_LIST})
separate_arguments(VERSION_LIST)

list(GET VERSION_LIST 0 CMAKE_PROJECT_VERSION_MAJOR)
list(GET VERSION_LIST 1 CMAKE_PROJECT_VERSION_MINOR)
list(GET VERSION_LIST 2 CPACK_PACKAGE_VERSION_PATCH)


add_definitions(-DVERSION_STRING="${VERSION_STRING}")
add_definitions(-DXTOSTRUCT_QT)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

message(" ")
message("Notepanda Version: ${VERSION_STRING}")
message("Notepanda Build Version: ${BUILD_VERSION}")
message("|-------------------------------------------------|")
message("| Notepanda, A Cross Platform Notepad.            |")
message("| Based on C++ & Qt.                              |")
message("| Licenced under MIT                              |")
message("|                                                 |")
message("| You may only use this program to the extent     |")
message("| permitted by local law.                         |")
message("|                                                 |")
message("| See: https://opensource.org/licenses/MIT        |")
message("|-------------------------------------------------|")
message("| GitHub: https://github.com/ChungZH/notepanda    |")
message("| Welcome to contribute!                          |")
message("|-------------------------------------------------|")
message(" ")


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

if(UNIX AND NOT APPLE AND NOT WIN32)
    install(TARGETS notepanda RUNTIME DESTINATION bin)
    install(FILES assets/notepanda.desktop DESTINATION share/applications)
    install(FILES images/notepanda.png DESTINATION share/icons/hicolor/256x256/apps)
endif()

target_link_libraries(notepanda Qt5::Widgets)