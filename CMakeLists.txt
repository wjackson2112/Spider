cmake_minimum_required(VERSION 3.18)

# set the project name
project(Spider)

if(EMSCRIPTEN)
#    add_compile_options("-s" "USE_GLFW=3")
#    add_compile_options("-s" "USE_FREETYPE=1")
#    add_compile_options("-s" "NODERAWFW=1")
    set(CMAKE_TOOLCHAIN_FILE $ENV{EMSDK_HOME}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s WASM=1 -O0 -s ASYNCIFY")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_FREETYPE=1 -s USE_PTHREADS -pthread ")
#    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -s USE_GLFW=3 -s USE_WEBGL2=1 -s PROXY_TO_PTHREAD -s MODULARIZE=1 -s EXPORT_NAME='Spider' -pthread  --emrun --preload-file assets --preload-file shaders -sSINGLE_FILE")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_FREETYPE=1 -O0 -g -fdebug-compilation-dir=${PROJECT_SOURCE_DIR}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} --shell-file ${PROJECT_SOURCE_DIR}/spider/build/shell_custom.html -s WASM=1 -s USE_GLFW=3 -s USE_WEBGL2=1 --preload-file assets --preload-file es_shaders@shaders -s ALLOW_MEMORY_GROWTH -gsplit-dwarf -gpubnames -gdwarf-5")

    set(CMAKE_EXECUTABLE_SUFFIX ".html") # This line is used to set your executable to build with the emscripten html template so taht you can directly open it.
endif()

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
