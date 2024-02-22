# Author: Kang Lin <kl222@126.com>

project(SerialPortAssistant)

set(HEADER_FILES
    MainWindow.h
    Global/Global.h
    Common/Tool.h
    SendFile.h
    ${CMAKE_SOURCE_DIR}/3th_lib/Control/lightbutton.h
    )
set(SOURCE_FILES
    MainWindow.cpp
    Main.cpp
    Global/Global.cpp
    Common/Tool.cpp
    SendFile.cpp
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
    OUT_REVISION SerialPortAssistant_Revision)
IF(NOT SerialPortAssistant_VERSION)
    SET(SerialPortAssistant_VERSION "v0.5.14")
ENDIF()
message("SerialPortAssistant_VERSION:${SerialPortAssistant_VERSION};Revision:${SerialPortAssistant_Revision}")

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
        RABBITCOMMON
    VERSION ${SerialPortAssistant_VERSION}
    )

INSTALL(FILES ${CMAKE_SOURCE_DIR}/etc/SerialPortAssistant_logqt.ini
        DESTINATION etc COMPONENT Runtime)

iF(WIN32)
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/Install/Install.nsi
        DESTINATION "${CMAKE_BINARY_DIR}"
            COMPONENT Runtime)
endif()
# Install other files
set(OTHER_FILES
    ${CMAKE_SOURCE_DIR}/License.md
    ${CMAKE_SOURCE_DIR}/Authors.md
    ${CMAKE_SOURCE_DIR}/ChangeLog.md
    ${CMAKE_SOURCE_DIR}/Authors_zh_CN.md
    ${CMAKE_SOURCE_DIR}/ChangeLog_zh_CN.md)
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
