#!/bin/bash

if [ -f /usr/lib/`uname -m`-linux-gnu/qt5/bin/qmake -a -z "$QT_ROOT" ]; then
    QT_ROOT=/usr/lib/`uname -m`-linux-gnu/qt5
fi

if [ -n "$1" ]; then
	QT_ROOT=$1
fi

if [ ! -f /usr/bin/qmake -a -z "$QT_ROOT" ]; then
	echo "$0 QT_ROOT RabbitCommon_DIR"
    exit -1
fi

if [ -n "$2" -a -z "$RabbitCommon_DIR" ]; then
	RabbitCommon_DIR=$2
fi

if [ -z "$RabbitCommon_DIR" ]; then
	RabbitCommon_DIR=`pwd`/../RabbitCommon
fi

if [ ! -d "$RabbitCommon_DIR" ]; then
	echo "$0 QT_ROOT RabbitCommon_DIR"
        exit -2
fi

export RabbitCommon_DIR=$RabbitCommon_DIR
export QT_ROOT=$QT_ROOT
dpkg-buildpackage -us -uc -b
