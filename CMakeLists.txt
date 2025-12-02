cmake_minimum_required(VERSION 3.11)
project(advent-of-code LANGUAGES C VERSION 0.1.0)

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Set default build type to Debug
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE
      Debug
      CACHE STRING "Build type" FORCE)
endif()

set(aoc_lib_sources
  src/lib/hash_map.c
  src/lib/macros.c
  src/lib/numeric.c
  src/lib/str.c
  src/lib/tree_map.c
  src/lib/vec.c
)

add_library(aoc_lib ${aoc_lib_sources})
target_include_directories(aoc_lib PUBLIC src)

set(aoc_day_sources
  src/2024/day_01_1.c
  src/2024/day_01_2.c
  src/2024/day_02_1.c
  src/2024/day_02_2.c

  src/2025/day_01_1.c
  src/2025/day_01_2.c
  src/2025/day_02_1.c
  src/2025/day_02_2.c
)

foreach(day_source ${aoc_day_sources})
  get_filename_component(day_name ${day_source} NAME_WE)
  get_filename_component(source_dir "${day_source}" DIRECTORY)
  get_filename_component(year "${source_dir}" NAME)

  set(exec_name "${year}_${day_name}")

  add_executable(${exec_name} ${day_source})
  target_link_libraries(${exec_name} PRIVATE aoc_lib)
endforeach()
