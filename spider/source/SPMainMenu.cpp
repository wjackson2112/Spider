//
// Created by Will on 12/2/2022.
//
#include "SPMainMenu.h"

#include "CameraEntity2D.h"
#include "MenuButton.h"
#include "SPBackground.h"
#include "EntityManager.h"
#include "EventManager.h"

#include "Inputs.h"
#include "InputManager.h"

SPMainMenu::SPMainMenu()
{
    std::vector<Key> flags = {KEY_SPACE};
    InputManager::getInstance()->registerReceiver(this, flags) ;

    EntityManager* entityManager = EntityManager::getInstance();
    entityManager->registerEntity(this, new CameraEntity2D());
    entityManager->registerEntity(this, new SPBackground());
    entityManager->registerEntity(this, new MenuButton(glm::vec2(50,50), glm::vec2(275, 50),
                                                       "assets\\block.png", "New Game", NEW_GAME));
    entityManager->registerEntity(this, new MenuButton(glm::vec2(50,125), glm::vec2(275, 50),
                                                       "assets\\block.png", "Quit Game", QUIT_GAME));
}

SPMainMenu::~SPMainMenu()
{
    InputManager::getInstance()->deregisterReceiver(this);
}

void SPMainMenu::keyInputCallback(Key key, int scancode, Action action, Modifier mods)
{
    if(key == KEY_SPACE)
        EventManager::getInstance()->broadcastEvent(NEW_GAME);
}