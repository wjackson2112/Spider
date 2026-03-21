# add the executable
add_executable(Spider source/NSGame.cpp #source/SPGame.cpp
                      source/SPMainMenu.cpp
                      source/NSScene.cpp #source/SPScene.cpp
#                      source/SPCard.cpp
                      source/NSGameMode.cpp
                      source/NSGameState.cpp
#                      source/SPSnapValidatorFourSuits.cpp # Still compiling this for now to get SPGameState
#                      source/SPPile.cpp
#                      source/SPPilable.cpp
                      source/SPFrameCounterEntity.cpp
                      source/SPCursor.cpp
#                      source/SPGameplayAction.cpp
#                      source/SPSelectAction.cpp
#                      source/SPDealAction.cpp
#                      source/SPCancelAction.cpp
#                      source/SPUndoAction.cpp
                      source/NSSelectAction.cpp
                      source/NSCancelAction.cpp
                      source/NSDealAction.cpp
                      source/NSUndoAction.cpp
                      source/NSResetAction.cpp)

target_include_directories(Spider
                           PUBLIC
                           source/include)

target_link_libraries(Spider
                      PUBLIC
                      CardFramework)

target_link_libraries(Spider
                      PUBLIC
                      GameEngine)

add_subdirectory(assets)

if(EMSCRIPTEN)
    add_subdirectory(es_shaders)
    target_link_options(Spider
            PRIVATE
            "-sEXPORTED_RUNTIME_METHODS=['requestFullscreen']")
else()
    add_subdirectory(shaders)
endif()

if(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++)
endif()

#add_executable(Spider source/SPGameStripped.cpp
#                      source/SPMainMenu.cpp
#                      source/SPBackground.cpp
#                      source/SPScene.cpp
#                      source/SPCard.cpp
#                      source/SPSnapValidatorFourSuits.cpp
#                      source/SPPile.cpp
#                      source/SPPilable.cpp
#                      source/SPFrameCounterEntity.cpp
#                      source/SPCursor.cpp)
#
#target_include_directories(Spider
#                           PUBLIC
#                           source/include)
#
#target_link_libraries(Spider
#                      PUBLIC
#                      GameEngine)
#
#add_subdirectory(assets)
#add_subdirectory(shaders)
#
#if(MINGW)
#    set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++)
#endif()