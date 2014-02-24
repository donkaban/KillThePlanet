#!/bin/bash
cd ./android

android update project --name com.kaban.asteroids --path . --target "android-19"
ndk-build
ant debug
ant installd

