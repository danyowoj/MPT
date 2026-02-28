#!/usr/bin/env bash

g++ -Iinclude -o calc_app main.cpp src/UFrac.cpp src/UEditor.cpp src/TProc.cpp src/TSettings.cpp src/THistory.cpp src/UCtrl.cpp
./calc_app
