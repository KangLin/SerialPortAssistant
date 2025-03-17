#!/bin/bash
# Author: Kang Lin <kl222@126.com>

#See: https://blog.csdn.net/alwaysbefine/article/details/114187380
set -x
set -e
#set -v

EXIT_CODE=0
PROJECT_NAME="SerialPortAssistant"

if [ -n "$1" ]; then
    echo "$PROJECT_NAME"
	PROJECT_NAME=$1
fi

if [ ! -f /opt/${PROJECT_NAME}/share/applications/io.github.KangLin.SerialPortAssistant.desktop ]; then
	echo "There are not /opt/share/applications/io.github.KangLin.SerialPortAssistant.desktop"
	EXIT_CODE=$[EXIT_CODE+1]
fi

if [ ! -f /usr/share/applications/io.github.KangLin.SerialPortAssistant.desktop ]; then
	echo "There are not /usr/share/applications/io.github.KangLin.SerialPortAssistant.desktop"
	EXIT_CODE=$[EXIT_CODE+1]
fi

if [ ! -f /usr/share/icons/hicolor/128x128/apps/io.github.KangLin.SerialPortAssistant.png ]; then
    echo "There are not /usr/share/icons/hicolor/128x128/apps/io.github.KangLin.SerialPortAssistant.png"
    EXIT_CODE=$[EXIT_CODE+1]
fi

exit $EXIT_CODE
