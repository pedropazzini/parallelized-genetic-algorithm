#!/bin/bash

clear
rm error
rm output

make -f makefile 2>error

if [ -f $error]; then
    vim error
else
    ./ga > output
    vim output
fi
