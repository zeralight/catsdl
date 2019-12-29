cmake_minimum_required(VERSION 3.10)
project(circle_the_cat)
set(CMAKE_C_STANDARD 11)


set(SOURCES
    src/main.c
    src/game.c
    src/gfx.c
    src/graph.c
    src/map.c
    src/scores.c
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/)

add_executable(${PROJECT_NAME} ${SOURCES})

find_package(Threads REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

target_link_libraries(${PROJECT_NAME} SDL2::Main SDL2::Image SDL2::TTF)