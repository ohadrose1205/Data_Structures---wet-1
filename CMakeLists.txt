cmake_minimum_required(VERSION 3.22)
project(wet_1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(__MACOSX)

add_executable(wet_1
        __MACOSX/._main23a1.cpp
        __MACOSX/._wet1util.h
        __MACOSX/._worldcup23a1.cpp
        __MACOSX/._worldcup23a1.h
        AVL_trees.cpp
        AVL_trees.h
        main23a1.cpp
        wet1util.h
        worldcup23a1.cpp
        worldcup23a1.h)
