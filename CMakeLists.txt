cmake_minimum_required(VERSION 3.23)
project(training C)

set(CMAKE_C_STANDARD 11)

include_directories(scripts/scripts)

add_executable(training
        scripts/scripts/DrawingDataSetter.h
        scripts/scripts/DrawingFunctions.h
        scripts/scripts/main.c
        scripts/scripts/WorkWithMatrices.h)
