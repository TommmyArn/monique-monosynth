#!/bin/bash
konsole -e /bin/sh -c "cd $(dirname "$0") && echo 'BUILD LINUX MONIQUE 32 STANDALONE' && make clean CONFIG=x64 && make CONFIG=x32 -j6 && notify-send 'BUILD LINUX MONIQUE 32 STANDALONE - DONE'" 
