cmake_minimum_required(VERSION 3.27)
project(Hylabirynt)

set(CMAKE_CXX_STANDARD 17)
find_package(SDL2 REQUIRED SDL2)
find_package(SDL2_ttf REQUIRED SDL2_ttf)
add_executable(Hylabirynt main.cpp)
target_link_libraries(Hylabirynt PRIVATE SDL2::SDL2)
target_link_libraries(Hylabirynt PRIVATE SDL2_ttf::SDL2_ttf)
