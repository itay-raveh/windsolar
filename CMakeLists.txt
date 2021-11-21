cmake_minimum_required(VERSION 3.17)
project(windsolar C)

set(CMAKE_C_STANDARD 11)

add_executable(windsolar Windsolar/main.c Windsolar/utils.c Windsolar/utils.h Windsolar/macros.h)