#include <iostream>

#include "Window.h"

#include "SPGame.h"
#include "SPMainMenu.h"
//#include "Level.h"
#include "EventManager.h"

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
        default:
            break;
    }
}

void SPGame::update()
{
//    if(shouldStartNewGame)
//    {
//        sceneStack.clearScenes();
//        Scene* newLevel = new Level("assets/level1.txt");
//        sceneStack.pushScene(newLevel);
//        shouldStartNewGame = false;
//    }

    Game::update();
}


int main()
{
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

        // Update entities
        spider.update();

        // Resolve collisions
        spider.resolveCollisions();
    }

    std::cout << "Terminating" << std::endl;

    // Terminate
    window.close();
    return 0;
}

