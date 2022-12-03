cmake_minimum_required(VERSION 3.21)
project(Project3)

set(CMAKE_CXX_STANDARD 14)

add_executable(Project3 src/Movie.h src/main.cpp src/GraphRecommend.cpp src/GraphRecommend.h src/HashRecommend.h src/HashRecommend.cpp)
