#!/bin/bash

rm -f  ./android/libs/
rm -f  ./android/obj/
rm -f  ./Asteroids

cd ./android
ndk-build clean
ant  clean
cd jni
make clean
cd ..
rm -f  ./build.xml
rm -f  ./local.properties
rm -f  ./proguard-project.txt
rm -f  ./project.properties




