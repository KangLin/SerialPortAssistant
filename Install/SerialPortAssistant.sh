#!/bin/bash

set -e

#获取运行的程序名
PRONAME=`basename $0`
#获取文件运行的当前目录
current_dir=$(cd "$(dirname "$0")"; pwd)

export LD_LIBRARY_PATH=$current_dir:$current_dir/../lib:$LD_LIBRARY_PATH
/opt/SerialPortAssistant/bin/SerialPortAssistant $*
