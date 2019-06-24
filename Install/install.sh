#!/bin/bash

# Use to install appimage in linux

case "$1" in
    remove)
        echo "remove ..."
        rm -f /usr/share/applications/SerialPortAssistant.desktop
        rm -f /usr/share/pixmaps/SerialPortAssistant.png
    ;;
    
    install|*)
        echo "install ..."
        if [ ! -f /usr/share/applications/SerialPortAssistant.desktop ]; then
            ln -s `pwd`/share/applications/SerialPortAssistant.desktop /usr/share/applications/SerialPortAssistant.desktop
            sed -i "s/Exec=.*//g" /usr/share/applications/SerialPortAssistant.desktop
            echo "Exec=`pwd`/SerialPortAssistant-x86_64.AppImage" >> /usr/share/applications/SerialPortAssistant.desktop
        fi
        if [ ! -f /usr/share/pixmaps/SerialPortAssistant.png ]; then
            if [ ! -d /usr/share/pixmaps ]; then
                mkdir -p /usr/share/pixmaps
            fi
            ln -s `pwd`/share/pixmaps/SerialPortAssistant.png /usr/share/pixmaps/SerialPortAssistant.png
        fi
        ;;
        
esac
