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

find_package(Qt5 COMPONENTS Widgets Svg REQUIRED)
find_package(KF5SyntaxHighlighting REQUIRED)

if(WIN32)
    set(GUI_TYPE WIN32)
elseif(APPLE)
    set(GUI_TYPE MACOSX_BUNDLE)
endif()

set(SOURCES
    src/main.cpp
    src/ui/aboutwindow.cpp
    src/ui/aboutwindow.h
    src/ui/aboutwindow.ui
    src/ui/mainwindow.cpp
    src/ui/mainwindow.h
    src/ui/linenumberarea.h
    src/ui/mainwindow.ui
    src/ui/preferenceswindow.cpp
    src/ui/preferenceswindow.h
    src/ui/preferenceswindow.ui
    src/core/texteditor.cpp
    src/core/texteditor.h
    assets/notepanda.rc
    ${CMAKE_SOURCE_DIR}/resources.qrc
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

include(InstallRequiredSystemLibraries)

# Packaging
set(CPACK_PACKAGE_VENDOR "ChungZH")
set(CPACK_PACKAGE_VERSION ${VERSION_STRING})
set(CPACK_PACKAGE_DESCRIPTION "A simple cross-platform notepad.")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/ChungZH/notepanda")
set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/images/panda.ico")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.md")


if(MSVC)
   if(NOT BUILD_NSIS)
    set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION .)
   endif()
endif()

if(MSVC)
    if(BUILD_NSIS)
        add_definitions(-DQV2RAY_NO_ASIDECONFIG)
        set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/images\\\\panda.ico")
        set(CPACK_GENERATOR "NSIS")
        set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/images/panda.ico")
        set(CPACK_NSIS_MUI_UNIICON "${CMAKE_SOURCE_DIR}/images/panda.ico")
        set(CPACK_NSIS_DISPLAY_NAME "Notepanda")
        set(CPACK_NSIS_PACKAGE_NAME "Notepanda")
        set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
            CreateShortCut \\\"$DESKTOP\\\\Notepanda.lnk\\\" \\\"$INSTDIR\\\\notepanda.exe\\\"
            CreateDirectory \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\Notepanda\\\"
            CreateShortCut \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\Notepanda\\\\Notepanda.lnk\\\" \\\"$INSTDIR\\\\notepanda.exe\\\"
            WriteRegStr HKLM \\\"Software\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\Uninstall\\\\notepanda\\\" \\\"DisplayIcon\\\" \\\"$INSTDIR\\\\notepanda.exe\\\"
            WriteRegStr HKLM \\\"Software\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\Uninstall\\\\notepanda\\\" \\\"HelpLink\\\" \\\"https://github.com/ChungZH/notepanda\\\"
            WriteRegStr HKLM \\\"Software\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\Uninstall\\\\notepanda\\\" \\\"InstallLocation\\\" \\\"$INSTDIR\\\"
            WriteRegStr HKLM \\\"Software\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\Uninstall\\\\notepanda\\\" \\\"URLUpdateInfo\\\" \\\"https://github.com/ChungZH/notepanda/releases\\\"
            WriteRegStr HKLM \\\"Software\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\Uninstall\\\\notepanda\\\" \\\"URLInfoAbout\\\" \\\"https://github.com/ChungZH/notepanda\\\"
        ")
        set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
            Delete \\\"$DESKTOP\\\\Notepanda.lnk\\\"
            Delete \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\Notepanda\\\\Notepanda.lnk\\\"
            RMDir \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\Notepanda\\\"
            DeleteRegKey HKLM \\\"Software\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\Uninstall\\\\notepanda\\\"
        ")
        set(CPACK_PACKAGE_INSTALL_DIRECTORY "notepanda")
    endif()
endif()

include(CPack)

target_link_libraries(notepanda Qt5::Widgets Qt5::Svg KF5::SyntaxHighlighting)
