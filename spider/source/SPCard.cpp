//
// Created by Will on 1/9/2023.
//

#include "SPCard.h"

#include <iostream>

#include "AssetManager.h"
#include "InputManager.h"
#include "SpriteSheetComponent2D.h"

SPCard::SPCard(glm::vec2 position, SPCardSuit suit, SPCardValue value)
: suit(suit)
, value(value)
, size(49.f, 64.f)
{
    this->transform.setPosition2(position);

    Shader shader = AssetManager::getInstance()->loadShader("shaders\\sprite.vert",
                                                            "shaders\\sprite.frag",
                                                            nullptr,
                                                            "sprite");
    // Deck asset is from here: https://www.codeproject.com/Articles/1187548/Video-Poker
    Texture2D texture = AssetManager::getInstance()->loadTexture("assets\\deck.png", true, "deck");

    auto* spriteComponent = new SpriteSheetComponent2D(shader, texture, size, glm::vec2(13, 4), value, suit);
    addComponent(spriteComponent);

    InputConfig config = InputConfig();
    config.mouseButtons.push_back(MOUSE_BUTTON_1);
    config.receivesMousePosition = true;
    InputManager::getInstance()->registerReceiver(this, config);
    receivesUpdates = true;
}

void SPCard::update(float deltaTime)
{
    Entity::update(deltaTime);
    InputManager* manager = InputManager::getInstance();
//    std::cout << "SPCARD::MOUSE PRESSED " << manager->isMouseButtonPressed(MOUSE_BUTTON_1) << std::endl;
}

void SPCard::mouseInputCallback(double xpos, double ypos, MouseButton button, Action action, Modifier mods)
{
    switch(action)
    {
        case ACTION_PRESS:
        {
            glm::vec2 cardPosition = this->getWorldTransform().getPosition2();
            if (xpos >= cardPosition.x && xpos <= cardPosition.x + this->size.x &&
                ypos >= cardPosition.y && ypos <= cardPosition.y + this->size.y)
            {
                grabPosition = glm::vec2(xpos, ypos); // Where the grab started
                grabOffset = glm::vec2(xpos - cardPosition.x, ypos - cardPosition.y); // The offset into the card the grab occurred
            }
            break;
        }
        case ACTION_RELEASE:
            grabPosition = NO_GRAB;
            grabOffset = NO_GRAB;
            break;
        case ACTION_NONE: // Action none means it's a mouse move
            // We don't care where the mouse is if the card isn't grabbed
            if(grabPosition == NO_GRAB || grabOffset == NO_GRAB)
                break;

            // Grabs are done only with mouse 1
            if(InputManager::getInstance()->getLastMouseState(MOUSE_BUTTON_1) != ACTION_PRESS)
                break;

            // A move while the button is pressed is a "drag"
            // check if we've moved past our minimum drag threshold, then move the card
            if(glm::length(glm::vec2(xpos, ypos) - grabPosition) > DRAG_THRESHOLD)
                this->getTransform()->setPosition2(glm::vec2(xpos - grabOffset.x, ypos - grabOffset.y));

            break;
    }
}

