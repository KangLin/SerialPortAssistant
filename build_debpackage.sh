#!/bin/bash

if [ -f /usr/bin/qmake -a -z "$QT_ROOT" ]; then
    QMAKE=qmake
fi

if [ -n "$1" ]; then
	QMAKE=$1
fi

if [ ! -f /usr/bin/qmake -a -z "$QT_ROOT" ]; then
	echo "$0 QMAKE RabbitCommon_DIR"
    exit -1
fi

if [ -n "$2" -a -z "$RabbitCommon_DIR" ]; then
	RabbitCommon_DIR=$2
fi

if [ -z "$RabbitCommon_DIR" ]; then
	RabbitCommon_DIR=`pwd`/../RabbitCommon
fi

if [ ! -d "$RabbitCommon_DIR" ]; then
	echo "$0 QMAKE RabbitCommon_DIR"
        exit -2
fi

export RabbitCommon_DIR=$RabbitCommon_DIR
export QMAKE=$QMAKE
dpkg-buildpackage -us -uc -b
#fakeroot debian/rules binary 
