cmake_minimum_required(VERSION 3.10)
project(Pilgrimage)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Pilgrimage
  main.cpp
  player.cpp
  game.cpp
  enemy.cpp
  config.cpp
)

if(WIN32)
  target_link_libraries(Pilgrimage))
else()
  target_link_libraries(Pilgrimage curses)
endif()