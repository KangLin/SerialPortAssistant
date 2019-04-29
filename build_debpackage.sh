#!/bin/bash

if [ -n "$1" -a -z "$Qt5_DIR" ]; then
	Qt5_DIR=$1
fi

if [ -z "$Qt5_DIR" ]; then
	echo "./$1 Qt5_DIR RabbitCommon_DIR"
    exit
fi

if [ -n "$2" -a -z "$RabbitCommon_DIR" ]; then
	RabbitCommon_DIR=$2
fi

if [ -z "$RabbitCommon_DIR" ]; then
	RabbitCommon_DIR=`pwd`/../RabbitCommon
fi

if [ ! -d "$RabbitCommon_DIR" ]; then
	echo "./$1 Qt5_DIR RabbitCommon_DIR"
	exit
fi

export RabbitCommon_DIR=$RabbitCommon_DIR
export Qt5_DIR=$Qt5_DIR
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$Qt5_DIR/../..
export LD_LIBRARY_PATH
fakeroot debian/rules binary 
#dpkg-buildpackage -us -uc -b

