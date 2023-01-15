# add the executable
add_executable(Spider source/SPGame.cpp
                      source/SPMainMenu.cpp
                      source/SPBackground.cpp
                      source/SPScene.cpp
                      source/SPCard.cpp
                      source/SPSnapValidatorFourSuits.cpp
                      source/SPSnapComponent.cpp)

target_include_directories(Spider
                           PUBLIC
                           source/include)

target_link_libraries(Spider
                      PUBLIC
                      GameEngine)

add_subdirectory(assets)
add_subdirectory(shaders)

if(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++)
endif()