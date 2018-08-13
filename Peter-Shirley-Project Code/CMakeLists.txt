cmake_minimum_required(VERSION 3.8)
project(Peter_Shirley_Project_Code)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp vec3.h ray.h hitable.h sphere.h hitable_list.h camera.h material.h)
add_executable(Peter_Shirley_Project_Code ${SOURCE_FILES})