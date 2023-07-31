# Author: Kang Lin <kl222@126.com>

cmake_minimum_required(VERSION 3.21)
MESSAGE(STATUS "Found CMake ${CMAKE_VERSION}")

project(SerialPortAssistant)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)
if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()
set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "verbose")

IF(MSVC)
    # This option is to enable the /MP switch for Visual Studio 2005 and above compilers
    OPTION(WIN32_USE_MP "Set to ON to build with the /MP option (Visual Studio 2005 and above)." ON)
    MARK_AS_ADVANCED(WIN32_USE_MP)
    IF(WIN32_USE_MP)
        #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
        add_compile_options(/MP)
    ENDIF(WIN32_USE_MP)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
ENDIF(MSVC)

SET(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libs")
if (BUILD_SHARED_LIBS)
    add_definitions(-DBUILD_SHARED_LIBS)
    if (CMAKE_COMPILER_IS_GNUCXX AND NOT MINGW)
       # Just setting CMAKE_POSITION_INDEPENDENT_CODE should be enough to set
       # -fPIC for GCC but sometimes it still doesn't get set, so make sure it
       # does.
       add_compile_options("-fPIC")
    endif()
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

# Open qt build tool
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)

# Need qt components
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
SET(QT_COMPONENTS Core Gui Widgets SerialPort Network Xml)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ${QT_COMPONENTS})
message("QT_VERSION:${Qt${QT_VERSION_MAJOR}_VERSION}")
if(Qt${QT_VERSION_MAJOR}_FOUND)
    FOREACH(_COMPONENT ${QT_COMPONENTS})
        list(APPEND QT_LIBRARIES Qt${QT_VERSION_MAJOR}::${_COMPONENT})
    ENDFOREACH()
endif()
get_filename_component(QT_INSTALL_DIR "${Qt${QT_VERSION_MAJOR}_DIR}/../../.." ABSOLUTE)
message("Qt${QT_VERSION_MAJOR}_DIR:${Qt${QT_VERSION_MAJOR}_DIR}")
message("QT_INSTALL_DIR:${QT_INSTALL_DIR}")
message("${PROJECT_NAME} QT_LIBRARIES:${QT_LIBRARIES}")

if(NOT RabbitCommon_DIR)
    set(RabbitCommon_DIR $ENV{RabbitCommon_DIR})
    if(NOT RabbitCommon_DIR)
        set(RabbitCommon_DIR ${CMAKE_SOURCE_DIR}/../RabbitCommon)
    endif()
endif()
if(DEFINED RabbitCommon_DIR AND EXISTS ${RabbitCommon_DIR}/Src)
    add_subdirectory(${RabbitCommon_DIR}/Src ${CMAKE_BINARY_DIR}/RabbitCommon)
else()
    message("1. Please download RabbitCommon source code from https://github.com/KangLin/RabbitCommon")
    message("   ag:")
    message("       git clone https://github.com/KangLin/RabbitCommon.git")
    message("2. Then set cmake value or environment variable RabbitCommon_DIR to download root directory.")
    message("   ag:")
    message(FATAL_ERROR "       cmake -DRabbitCommon_DIR= ")
endif()

GET_VERSION(OUT_VERSION SerialPortAssistant_VERSION
    OUT_REVISION SerialPortAssistant_REVISION)
IF(NOT SerialPortAssistant_VERSION)
    SET(SerialPortAssistant_VERSION "v0.5.14")
ENDIF()
message("SerialPortAssistant_VERSION:${SerialPortAssistant_VERSION};Revision:${SerialPortAssistant_REVISION}")

add_subdirectory(App)

INSTALL(FILES ${CMAKE_SOURCE_DIR}/etc/SerialPortAssistant_logqt.ini
        DESTINATION etc COMPONENT Runtime)

iF(WIN32)
    INSTALL(FILES Install/Install.nsi DESTINATION "${CMAKE_BINARY_DIR}"
        COMPONENT Runtime)
endif()
# Install other files
set(OTHER_FILES
    License.md
    Authors.md
    ChangeLog.md
    Authors_zh_CN.md
    ChangeLog_zh_CN.md)
if(ANDROID)
    INSTALL(FILES ${OTHER_FILES} DESTINATION "assets" COMPONENT Runtime)
else()
    INSTALL(FILES ${OTHER_FILES} DESTINATION "." COMPONENT Runtime)
endif()

INSTALL(FILES ${CMAKE_SOURCE_DIR}/App/SerialPortAssistant.ico
    DESTINATION "."
        COMPONENT Runtime)

if(UNIX AND NOT ANDROID)
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/share/SerialPortAssistant.desktop
        DESTINATION "share/applications"
            COMPONENT Runtime)
    INSTALL(PROGRAMS ${CMAKE_SOURCE_DIR}/share/SerialPortAssistant.sh
        DESTINATION "${CMAKE_INSTALL_BINDIR}"
            COMPONENT Runtime)
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/App/Resource/png/SerialPortAssistant.png
        DESTINATION "share/pixmaps"
            COMPONENT Runtime)
endif()
