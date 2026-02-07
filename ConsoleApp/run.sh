#!/bin/bash

g++ -std=c++17 main.cpp AbonentList.cpp Interface.cpp FileManager.cpp Help.cpp -o phonebook
./phonebook
