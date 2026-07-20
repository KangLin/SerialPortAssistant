# Author: Kang Lin <kl222@126.com>

project(SerialPortAssistant)

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    # 32 位系统
    #add_definitions(QT_NO_64BIT_ATOMICS)
    set(SerialPortAssistant_DEFINED QT_NO_64BIT_ATOMICS)
    message(STATUS "Building for 32-bit, disabling 64-bit atomics")
else()
    message(STATUS "Building for 64-bit")
endif()

set(HEADER_FILES
    MainWindow.h
    Global/Global.h
    Common/Tool.h
    SendFile.h
    Stats.h
    ${CMAKE_SOURCE_DIR}/3th_lib/Control/lightbutton.h
    )
set(SOURCE_FILES
    MainWindow.cpp
    Main.cpp
    Global/Global.cpp
    Common/Tool.cpp
    SendFile.cpp
    Stats.cpp
    ${CMAKE_SOURCE_DIR}/3th_lib/Control/lightbutton.cpp
    )
set(SOURCE_UI_FILES
    MainWindow.ui
    )
set(RESOURCE_FILES Resource/Resource.qrc)
if(WIN32)
    list(APPEND RESOURCE_FILES AppIcon.rc)
endif()

GET_VERSION(OUT_VERSION SerialPortAssistant_VERSION
    OUT_REVISION SerialPortAssistant_Revision
    OUT_TAG SerialPortAssistantTAG)
IF(NOT SerialPortAssistant_VERSION)
    SET(SerialPortAssistant_VERSION "0.5.33")
ENDIF()
if(NOT(SerialPortAssistant_VERSION MATCHES "[0-9]+\\.[0-9]+\\.[0-9]+[+\\-^_~0-9A-Za-z]*"))
    SET(SerialPortAssistant_VERSION "0.5.33")
endif()
IF(NOT SerialPortAssistantTAG)
    SET(SerialPortAssistantTAG "0.5.33")
ENDIF()
message("SerialPortAssistant_VERSION:${SerialPortAssistant_VERSION};Revision:${SerialPortAssistant_Revision};Tag:${SerialPortAssistantTAG}")

set(APP_QT_COMPONENTS Core Gui Widgets Network Svg Xml)
if(QT_VERSION_MAJOR VERSION_GREATER_EQUAL 6)
    list(APPEND APP_QT_COMPONENTS StateMachine)
endif()
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ${APP_QT_COMPONENTS})
if(Qt${QT_VERSION_MAJOR}_FOUND)
    FOREACH(_COMPONENT ${APP_QT_COMPONENTS})
        list(APPEND QT_LIBRARIES Qt${QT_VERSION_MAJOR}::${_COMPONENT})
    ENDFOREACH()
else()
    message(FATAL_ERROR "Qt${QT_VERSION_MAJOR} don't found")
endif()

set(APP_ID io.github.KangLin.SerialPortAssistant)

ADD_TARGET(NAME ${PROJECT_NAME}
    ISEXE
    ISWINDOWS
    SOURCE_FILES ${SOURCE_FILES} ${SOURCE_UI_FILES} ${HEADER_FILES} ${RESOURCE_FILES}
    INCLUDE_DIRS
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> # See: http://www.it1352.com/478094.html
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/3th_lib/Control>
    PRIVATE_LIBS RabbitCommon ${QT_LIBRARIES}
    PRIVATE_DEFINITIONS SerialPortAssistant_VERSION="${SerialPortAssistant_VERSION}"
        SerialPortAssistant_Revision="${SerialPortAssistant_Revision}"
        RABBITCOMMON ${SerialPortAssistant_DEFINED}
    VERSION ${SerialPortAssistant_VERSION}
    )

INSTALL_FILE(SOURCES ${CMAKE_SOURCE_DIR}/etc/SerialPortAssistant_logqt.ini
    DESTINATION etc
        COMPONENT Application)

iF(WIN32)
    # 替换 Install.nsi 中的 CMAKE_INSTALL_PREFIX 等
    configure_file(${CMAKE_SOURCE_DIR}/Install/Install.nsi
        ${CMAKE_BINARY_DIR}/Install.nsi @ONLY)
endif()

# Install other files
set(OTHER_FILES
    ${CMAKE_SOURCE_DIR}/License.md
    ${CMAKE_SOURCE_DIR}/Authors.md
    ${CMAKE_SOURCE_DIR}/ChangeLog.md
    ${CMAKE_SOURCE_DIR}/Authors_zh_CN.md
    ${CMAKE_SOURCE_DIR}/ChangeLog_zh_CN.md)
INSTALL_FILE(SOURCES ${OTHER_FILES}
    DESTINATION ${CMAKE_INSTALL_DOCDIR}
        COMPONENT Application)

if(ANDROID)

    if(NOT SerialPortAssistant_VERSION)
        message(WARNING "SerialPortAssistant_VERSION is empty, using default version")
        set(SerialPortAssistant_VERSION "1.0.0")
    endif()

    # 确保版本号格式正确
    if(NOT SerialPortAssistant_VERSION MATCHES "^[0-9]+\\.[0-9]+\\.[0-9]+")
        message(WARNING "Version format is not 'x.y.z', using fallback parsing")
    endif()

    # 使用方案一的解析逻辑
    string(REGEX MATCHALL "[0-9]+" VERSION_PARTS "${SerialPortAssistant_VERSION}")
    list(LENGTH VERSION_PARTS PARTS_COUNT)

    if(PARTS_COUNT GREATER_EQUAL 3)
        list(GET VERSION_PARTS 0 VERSION_MAJOR)
        list(GET VERSION_PARTS 1 VERSION_MINOR)
        list(GET VERSION_PARTS 2 VERSION_PATCH)
    else()
        # 不足3段时补齐
        set(VERSION_MAJOR 1)
        set(VERSION_MINOR 0)
        set(VERSION_PATCH 0)
    endif()

    math(EXPR QT_ANDROID_VERSION_CODE "${VERSION_MAJOR}*10000 + ${VERSION_MINOR}*100 + ${VERSION_PATCH}")
    set(QT_ANDROID_VERSION_NAME "${SerialPortAssistant_VERSION}")

    message(STATUS "${PROJECT_NAME} Version Code: ${QT_ANDROID_VERSION_CODE}, Version Name: ${QT_ANDROID_VERSION_NAME}")

    set_target_properties(${PROJECT_NAME} PROPERTIES
        QT_ANDROID_PACKAGE_NAME "${APP_ID}"
        QT_ANDROID_APP_NAME "@string/app_name"
        QT_ANDROID_APP_ICON "@drawable/icon"
        QT_ANDROID_VERSION_CODE ${QT_ANDROID_VERSION_CODE}
        QT_ANDROID_VERSION_NAME ${QT_ANDROID_VERSION_NAME}
        #QT_ANDROID_MIN_SDK_VERSION 28
        QT_ANDROID_TARGET_SDK_VERSION 34
        )
    qt_add_android_permission(${PROJECT_NAME}
        NAME android.permission.WRITE_EXTERNAL_STORAGE
    )
    qt_add_android_permission(${PROJECT_NAME}
        NAME android.permission.READ_EXTERNAL_STORAGE
    )

elseif(WIN32)
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/App/SerialPortAssistant.ico
        DESTINATION "."
            COMPONENT Application)
elseif(APPLE)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Resource/App.Info.plist
        ${CMAKE_CURRENT_BINARY_DIR}/Info.plist
                 @ONLY)
    INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/Info.plist
        DESTINATION "." COMPONENT Application)
    target_add_resources(${PROJECT_NAME} . "${CMAKE_CURRENT_SOURCE_DIR}/Resource/${APP_ID}.icns")
elseif(UNIX AND NOT ANDROID)
    configure_file(${CMAKE_SOURCE_DIR}/share/applications/${APP_ID}.desktop.in
        ${CMAKE_BINARY_DIR}/${APP_ID}.desktop @ONLY)
    INSTALL(FILES ${CMAKE_BINARY_DIR}/${APP_ID}.desktop
        DESTINATION "${CMAKE_INSTALL_DATADIR}/applications"
            COMPONENT Application)
    # INSTALL(PROGRAMS ${CMAKE_SOURCE_DIR}/share/SerialPortAssistant.sh
    #     DESTINATION "${CMAKE_INSTALL_BINDIR}"
    #         COMPONENT Application)
    # Icon path: http://freedesktop.org/wiki/Standards/icon-theme-spec
    # The freedesktop.org standard specifies in which order and directories programs should look for icons:
    #
    #    $HOME/.icons (for backwards compatibility)
    #    $XDG_DATA_DIRS/icons
    #    /usr/share/pixmaps
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/App/Resource/png/SerialPortAssistant.png
        DESTINATION ${CMAKE_INSTALL_DATADIR}/pixmaps
            COMPONENT Application
        RENAME ${APP_ID}.png)
    # Flatpak: https://docs.flatpak.org/en/latest/conventions.html#application-icons
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/App/Resource/png/SerialPortAssistant.png
        DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/128x128/apps
            COMPONENT Application
        RENAME ${APP_ID}.png)

    string(TIMESTAMP BUILD_DATE "%Y-%m-%d")
    configure_file(${CMAKE_SOURCE_DIR}/share/metainfo/${APP_ID}.metainfo.xml.in
        ${CMAKE_BINARY_DIR}/${APP_ID}.metainfo.xml @ONLY)
    INSTALL(FILES ${CMAKE_BINARY_DIR}/${APP_ID}.metainfo.xml
        DESTINATION "${CMAKE_INSTALL_DATADIR}/metainfo"
            COMPONENT Application)
endif()
