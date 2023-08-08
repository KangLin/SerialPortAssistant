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
