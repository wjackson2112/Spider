//  Copyright (c) 2026 William Jackson
//  Licensed under the MIT License

#include <iostream>

#include "Window.h"

#include "SPGame.h"
#include "SPMainMenu.h"
#include "SPScene.h"
#include "EventManager.h"
#include "InputManager.h"
#ifdef USE_STEAMWORKS
#include "Steamworks.h"
#endif

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define GL_GLEXT_PROTOTYPES
#define EGL_EGLEXT_PROTOTYPES
#endif

#include <functional>

SPGame::SPGame()
//        : Game(new MainMenu())
        : Game(new SPMainMenu())
//          : Game(new SPScene())
{
    EventManager::getInstance()->registerReceiver(this);
}

void SPGame::eventCallback(Event event)
{
    switch(event)
    {
        case Event::EVT_NEW_GAME:
        {
            shouldStartNewGame = true;
            break;
        }
        case Event::EVT_WON_GAME:
        {
            shouldReturnToMenu = true;
        }
        default:
            break;
    }
}

void SPGame::update()
{
    if(shouldStartNewGame)
    {
        sceneStack.clearScenes();
        InputManager::getInstance()->clearBindings();
        Scene* mainScene = new SPScene();
        sceneStack.pushScene(mainScene);
        shouldStartNewGame = false;
    }
    // TODO: This causes a crash as the entity objects are cleaned up
    else if(shouldReturnToMenu)
    {
        sceneStack.clearScenes();
        InputManager::getInstance()->clearBindings();
        Scene* menuScene = new SPMainMenu();
        sceneStack.pushScene(menuScene);
        shouldReturnToMenu = false;
    }

    Game::update();
}

std::function<void()> loop;
void main_loop() { loop(); }

int main()
{
#ifdef USE_STEAMWORKS
    if ( SteamAPI_RestartAppIfNecessary( 480 ) )
        return 1;

    Steamworks steamworks_sdk;
    if(steamworks_sdk.init() || !SteamUser()->BLoggedOn())
    {
        std::cout << "Failed to initialize Steamworks" << std::endl;
        return 1;
    }

    SteamClient()->SetWarningMessageHook( &SteamAPIDebugTextHook );
#endif

    auto window = Window();
    
    SPGame spider = SPGame();

    loop = [&] {
        spider.draw();

        // Double buffer
        window.swapBuffers();

        // Poll GL Events
        window.processInput();

#ifdef USE_STEAMWORKS
        steamworks_sdk.update();
#endif

        // Update entities
        spider.update();

        // Resolve collisions
        spider.resolveCollisions();
    };

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, true);
#else
    while(!window.shouldClose())
    {
        main_loop();
    }
#endif

    std::cout << "Terminating" << std::endl;

    // Terminate
#ifdef USE_STEAMWORKS
    SteamAPI_Shutdown();
#endif
    window.close();
    return 0;
}

