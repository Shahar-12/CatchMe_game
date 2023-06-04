cmake_minimum_required(VERSION 3.24)
project(packman C)

set(CMAKE_C_STANDARD 99)

set(SDL2_INCLUDE_DIR C:\\libs\\SDL\\include)
set(SDL2_LIB_DIR C:\\libs\\SDL\\lib\\x64)
set(SDL2_TTF_INCLUDE_DIR C:\\libs\\ttf\\include)
set(SDL2_TTF_LIB_DIR C:\\libs\\ttf\\lib\\x64)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIR})
LINK_DIRECTORIES(${SDL2_LIB_DIR})
INCLUDE_DIRECTORIES(${SDL2_TTF_INCLUDE_DIR})
LINK_DIRECTORIES(${SDL2_TTF_LIB_DIR})

add_executable(packman main.c)
target_link_libraries(${PROJECT_NAME} SDL2main SDL2)
target_link_libraries(${PROJECT_NAME} SDL2_ttf)
