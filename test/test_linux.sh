#!/bin/bash

EXIT_CODE=0
PROJECT_NAME="SerialPortAssistant"

if [ -n "$1" ]; then
    echo "$PROJECT_NAME"
	PROJECT_NAME=$1
fi

if [ ! -f /opt/${PROJECT_NAME}/share/applications/org.Rabbit.${PROJECT_NAME}.desktop ]; then
	echo "There are not /opt/share/applications/org.Rabbit.${PROJECT_NAME}.desktop"
	EXIT_CODE=$[EXIT_CODE+1]
fi

if [ ! -f /usr/share/applications/org.Rabbit.${PROJECT_NAME}.desktop ]; then
	echo "There are not /usr/share/applications/org.Rabbit.${PROJECT_NAME}.desktop"
	EXIT_CODE=$[EXIT_CODE+1]
fi

if [ ! -f /opt/${PROJECT_NAME}/share/pixmaps/${PROJECT_NAME}.png ]; then
    echo "There are not /opt/${PROJECT_NAME}/share/pixmaps/${PROJECT_NAME}.png"
	EXIT_CODE=$[EXIT_CODE+1]
fi

if [ ! -f /usr/share/pixmaps/org.Rabbit.${PROJECT_NAME}.png ]; then
    echo "There are not /usr/share/pixmaps/org.Rabbit.${PROJECT_NAME}.png"
    EXIT_CODE=$[EXIT_CODE+1]
fi

if [ ! -f "/opt/${PROJECT_NAME}/bin/${PROJECT_NAME}.sh" ]; then
    echo "/opt/${PROJECT_NAME}/bin/${PROJECT_NAME}.sh"
    EXIT_CODE=$[EXIT_CODE+1]
fi

#if [ ! -f /etc/ld.so.conf.d/${PROJECT_NAME}.conf ]; then
#	echo "There are not /etc/ld.so.conf.d/${PROJECT_NAME}.conf"
#	EXIT_CODE=$[EXIT_CODE+1]
#fi

exit $EXIT_CODE
