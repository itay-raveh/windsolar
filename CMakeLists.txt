cmake_minimum_required(VERSION 3.17)
project(windsolar C)

IF (DEBUG)
    add_definitions(-DDEBUG=1)
ENDIF (DEBUG)

set(CMAKE_C_STANDARD 11)

add_executable(windsolar Windsolar/main.c Windsolar/utils.c Windsolar/utils.h Windsolar/macros.h Windsolar/reader.c Windsolar/reader.h Windsolar/token.h Windsolar/token.c Windsolar/tokenizer.c Windsolar/tokenizer.h Windsolar/error.c Windsolar/error.h Windsolar/parse_tree.c Windsolar/parse_tree.h Windsolar/stacks.h Windsolar/commands.c Windsolar/commands.h Windsolar/runtime.c Windsolar/runtime.h Windsolar/args.h Windsolar/stacks.c Windsolar/args.c)
add_compile_options(-Wall -Wextra -Wpedantic -Werror)
