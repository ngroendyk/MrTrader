#!/bin/bash
# Written By: Nate Groendyk.
# Date: 20 Oct 2018
# Purpose: Wipe out cmake generated stuff and build stuff.

rm cmake_install.cmake
rm -rf ./CMakeFiles
rm CMakeCache.txt

rm *.exc
rm Makefile
 
