cmake_minimum_required(VERSION 2.8)

## Use the variable PROJECT_NAME for changing the target name
set( PROJECT_NAME "kernel" )

## Set our project name
project(${PROJECT_NAME})

## Use all the *.cpp files we found under this folder for the project
FILE(GLOB SRCS "*.cpp" "*.c" "*.s")

set ( CMAKE_CXX_FLAGS, "-std=gnu99 -ffreestanding -O2 -Wall -Wextra")

## Define the executable
add_executable(${PROJECT_NAME}.bin ${SRCS})
