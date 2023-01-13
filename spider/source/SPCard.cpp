//
// Created by Will on 1/9/2023.
//

#include "SPCard.h"

#include <iostream>

#include "AssetManager.h"
#include "InputComponent.h"
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
    auto* inputComponent = new InputComponent(config);
    addComponent(inputComponent);

    receivesUpdates = true;
}

void SPCard::update(float deltaTime)
{
    Entity::update(deltaTime);

    auto* inputComponent = getComponent<InputComponent>();

    while(inputComponent->hasMouseEvents())
    {
        MouseEvent event = inputComponent->dequeueMouseEvent();
        switch(event.action)
        {
            case ACTION_PRESS:
            {
                glm::vec2 cardPosition = this->getWorldTransform().getPosition2();
                if (event.position.x >= cardPosition.x && event.position.x <= cardPosition.x + this->size.x &&
                    event.position.y >= cardPosition.y && event.position.y <= cardPosition.y + this->size.y)
                {
                    grabPosition = event.position; // Where the grab started
                    grabOffset = glm::vec2(grabPosition.x - cardPosition.x, grabPosition.y - cardPosition.y); // The offset into the card the grab occurred
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
                if(glm::length(glm::vec2(event.position.x, event.position.y) - grabPosition) > DRAG_THRESHOLD)
                    this->getTransform()->setPosition2(glm::vec2(event.position.x - grabOffset.x, event.position.y - grabOffset.y));

                break;
            default:
                break;
        }
    }
}

