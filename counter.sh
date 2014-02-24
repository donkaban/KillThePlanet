#!/bin/sh

wc -l `find ./android/ -name '*.h' -o -name '*.hpp' -o -name '*.c' -o -name '*.cpp' -o -name '*.java'`