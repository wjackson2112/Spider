cmake_minimum_required(VERSION 3.18)

# set the project name
project(Spider)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
    set(BUILD_STATIC_LIBS OFF CACHE STRING "" FORCE)
    set(BUILD_SHARED_LIBS ON CACHE STRING "" FORCE)
else()
    set(BUILD_STATIC_LIBS ON CACHE STRING "" FORCE)
    set(BUILD_SHARED_LIBS OFF CACHE STRING "" FORCE)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/spider CACHE STRING "Path for runtime output files" FORCE)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/spider CACHE STRING "Path for runtime output files" FORCE)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/spider/lib CACHE STRING "Path for runtime output files" FORCE)

add_subdirectory(engine)
add_subdirectory(spider)
