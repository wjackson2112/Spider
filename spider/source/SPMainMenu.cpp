//
// Created by Will on 12/2/2022.
//
#include "SPMainMenu.h"

#include "CameraEntity2D.h"
#include "MenuButton.h"
#include "CFBackground.h"
#include "EventManager.h"

#include "InputManager.h"

SPMainMenu::SPMainMenu()
{
    addEntity<CameraEntity2D>();
    addEntity<CFBackground>();
    addEntity<MenuButton>(glm::vec2(50,50), glm::vec2(275, 50),
                          "assets/block.png", "New Game", Event(Event::EVT_NEW_GAME));
    addEntity<MenuButton>(glm::vec2(50,125), glm::vec2(275, 50),
                          "assets/block.png", "Quit Game", Event(Event::EVT_QUIT_GAME));

    InputManager::getInstance()->addBinding(MOUSE_BUTTON_1, BUTTON_ACTION_RELEASE, Event(Event::EVT_RELEASE));

//    std::cout << "Creating Main Menu" << std::endl;
//    EntityManager* entityManager = EntityManager::getInstance();
//    std::cout << "Creating Entity Manager" << std::endl;
//    entityManager->registerEntity(this, new CameraEntity2D());
//    std::cout << "Registered Camera" << std::endl;
//    entityManager->registerEntity(this, new SPBackground());
//    std::cout << "Registered Background" << std::endl;
//    entityManager->registerEntity(this, new MenuButton(glm::vec2(50,50), glm::vec2(275, 50),
//                                                       "assets/block.png", "New Game", NEW_GAME));
//    std::cout << "Registered New Game" << std::endl;
//    entityManager->registerEntity(this, new MenuButton(glm::vec2(50,125), glm::vec2(275, 50),
//                                                       "assets/block.png", "Quit Game", QUIT_GAME));
//    std::cout << "Registered Quit Game" << std::endl;
}