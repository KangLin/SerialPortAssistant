# Author: Kang Lin <kl222@126.com>

cmake_minimum_required(VERSION 3.21)
MESSAGE(STATUS "Found CMake ${CMAKE_VERSION}")

project(SerialPortAssistant)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "verbose")
# Need qt components
if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

include(GNUInstallDirs)

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
SET(QT_COMPONENTS Core Gui Widgets SerialPort Network)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ${QT_COMPONENTS})
message("QT_VERSION:${Qt${QT_VERSION_MAJOR}_VERSION}")
if(Qt${QT_VERSION_MAJOR}_FOUND)
    FOREACH(_COMPONENT ${QT_COMPONENTS})
        list(APPEND QT_LIBRARIES Qt${QT_VERSION_MAJOR}::${_COMPONENT})
    ENDFOREACH()
endif()
get_filename_component(QT_INSTALL_DIR "${Qt${QT_VERSION_MAJOR}_DIR}/../../.." ABSOLUTE)
message("QT_ROOT:${QT_ROOT}")
message("Qt${QT_VERSION_MAJOR}_ROOT:${Qt${QT_VERSION_MAJOR}_ROOT}")
message("Qt${QT_VERSION_MAJOR}_DIR:${Qt${QT_VERSION_MAJOR}_DIR}")
message("QT_INSTALL_DIR:${QT_INSTALL_DIR}")
message("QT_LIBRARIES:${QT_LIBRARIES}")

message("CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
message("CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")
message("CMAKE_FIND_USE_CMAKE_SYSTEM_PATH:${CMAKE_FIND_USE_CMAKE_SYSTEM_PATH}; NO_CMAKE_SYSTEM_PATH:${NO_CMAKE_SYSTEM_PATH}")
message("CMAKE_SYSTEM_PREFIX_PATH:${CMAKE_SYSTEM_PREFIX_PATH}")
if(NOT RabbitCommon_ROOT)
    set(RabbitCommon_ROOT $ENV{RabbitCommon_ROOT})
    if(NOT RabbitCommon_ROOT)
        set(RabbitCommon_ROOT ${CMAKE_SOURCE_DIR}/../RabbitCommon)
    endif()
endif()
if(RabbitCommon_ROOT AND EXISTS ${RabbitCommon_ROOT}/Src/CMakeLists.txt)
    message("Use RabbitCommon source code")
    add_subdirectory(${RabbitCommon_ROOT}/Src ${CMAKE_BINARY_DIR}/RabbitCommon)
else()
    find_package(RabbitCommon)
    if(NOT RabbitCommon_FOUND)
        message("RabbitCommon_ROOT is not found. Please use one of the following ways to set it:")
        message("1. Set RabbitCommon_ROOT to the install prefix of RabbitCommon.")
        message("2. Set RabbitCommon_ROOT to source code root of RabbitCommon.")
        message("2.1 Please download the source code of RabbitCommon from https://github.com/KangLin/RabbitCommon")
        message("   ag:")
        message("       git clone https://github.com/KangLin/RabbitCommon.git")
        message("2.2 Then set cmake variable or environment variable RabbitCommon_ROOT to download root directory.")
        message("   ag:")
        message("       cmake -DRabbitCommon_ROOT= ")
        message(FATAL_ERROR "RabbitCommon_ROOT isn't set.")
    endif()
endif()

add_subdirectory(App)
