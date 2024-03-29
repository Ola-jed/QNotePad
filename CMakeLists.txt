cmake_minimum_required(VERSION 3.17)
project(QNotePad)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets)
if( NOT Qt6_FOUND )
    set(QT_VERSION 5)
else()
    set(QT_VERSION 6)
endif()

set(REQUIRED_LIBS Core Gui Widgets)
set(REQUIRED_LIBS_QUALIFIED Qt${QT_VERSION}::Core Qt${QT_VERSION}::Gui Qt${QT_VERSION}::Widgets)

add_executable(${PROJECT_NAME} main.cpp src/notepad.cpp include/notepad.hpp src/about.cpp include/about.hpp src/settings.cpp include/settings.hpp src/popup.cpp include/popup.hpp include/keywords.hpp resources.qrc include/recentfilesmanager.hpp src/recentfilesmanager.cpp src/filemanager.cpp include/filemanager.hpp src/thememanager.cpp include/thememanager.hpp include/lazy.hpp src/fileextensionsloader.cpp include/fileextensionsloader.hpp include/utils.hpp src/editor.cpp include/editor.hpp src/linenumber.cpp include/linenumber.hpp)

if (NOT CMAKE_PREFIX_PATH)
    message(WARNING "CMAKE_PREFIX_PATH is not defined, you may need to set it "
            "(-DCMAKE_PREFIX_PATH=\"path/to/Qt/lib/cmake\" or -DCMAKE_PREFIX_PATH=/usr/include/{host}/qt{version}/ on Ubuntu)")
endif ()

if ( CMAKE_COMPILER_IS_GNUCC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Wunused")
endif()
if ( MSVC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4")
endif()
configure_file(lang_extensions.json  ${CMAKE_BINARY_DIR} COPYONLY)
include_directories(${QNotePad_SOURCE_DIR}/include)
find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)
target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED})