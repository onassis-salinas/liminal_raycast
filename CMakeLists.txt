cmake_minimum_required(VERSION 3.16)
project(liminal_raycast C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(game WIN32 src/main.c src/render.c src/walls.c src/maps.c src/textures.c)

if(EMSCRIPTEN)
  set_target_properties(game PROPERTIES SUFFIX ".html")
  target_compile_options(game PRIVATE "SHELL:-sUSE_SDL=3")
  target_link_options(game PRIVATE
    "SHELL:-sUSE_SDL=3"
    "SHELL:-sALLOW_MEMORY_GROWTH=1"
    "SHELL:--shell-file=${CMAKE_SOURCE_DIR}/src/shell.html"
  )
else()
  find_package(SDL3 REQUIRED CONFIG)
  target_link_libraries(game PRIVATE SDL3::SDL3)
endif()
