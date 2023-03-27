#include <iostream>

#include "Window.h"

#include "SPGame.h"
#include "SPMainMenu.h"
#include "SPScene.h"
#include "EventManager.h"
#include "Steamworks.h"


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

    Game::update();
}


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

    auto window = Window(false);
    
    SPGame spider = SPGame();

    std::cout << "Entering main loop" << std::endl;

    // Render loop
    while(!window.shouldClose())
    {
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
    }

    std::cout << "Terminating" << std::endl;

    // Terminate
    SteamAPI_Shutdown();
    window.close();
    return 0;
}

