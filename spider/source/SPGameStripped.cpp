#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define GL_GLEXT_PROTOTYPES
#define EGL_EGLEXT_PROTOTYPES
#endif

#include "Window.h"

#include <functional>

#include "SPGame.h"
#include "SPMainMenu.h"
#include "SPScene.h"
#include "EventManager.h"
//#ifdef USE_STEAMWORKS
//#include "Steamworks.h"
//#endif

SPGame::SPGame()
//        : Game(new MainMenu())
        : Game(new SPMainMenu())
{
    EventManager::getInstance()->registerReceiver(this);
}

void SPGame::eventCallback(Event event)
{
    switch(event)
    {
        case NEW_GAME:
        {
            shouldStartNewGame = true;
            break;
        }
        case WON_GAME:
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
        Scene* mainScene = new SPScene();
        sceneStack.pushScene(mainScene);
        shouldStartNewGame = false;
    }
    // TODO: This causes a crash as the entity objects are cleaned up
    else if(shouldReturnToMenu)
    {
        sceneStack.clearScenes();
        Scene* menuScene = new SPMainMenu();
        sceneStack.pushScene(menuScene);
        shouldReturnToMenu = false;
    }

    std::cout << "Updating" << std::endl;

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
    std::cout << "Starting" << std::endl;

    auto window = Window(false);

    std::cout << "Window Created" << std::endl;

    SPGame spider = SPGame();

    std::cout << "Game Created" << std::endl;

    loop = [&] {
        std::cout << "Looping" << std::endl;
        spider.draw();

        // Double buffer
        window.swapBuffers();

        // Poll GL Events
        window.processInput();
//
//#ifdef USE_STEAMWORKS
//        steamworks_sdk.update();
//#endif
//
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

