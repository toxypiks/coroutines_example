cmake_minimum_required(VERSION 3.16)
project(music_visualizer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(MAIN
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
  )

add_executable(main)
target_sources(main
 PRIVATE
  ${MAIN}
  )
