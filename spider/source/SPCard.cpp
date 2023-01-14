//
// Created by Will on 1/9/2023.
//

#include "SPCard.h"

#include <iostream>

#include "AssetManager.h"
#include "InputComponent.h"
//#include "TextComponent.h"
#include "SpriteSheetComponent2D.h"
#include "CollisionComponent2DAABB.h"

#define STACK_OFFSET 0.0001f
#define STACK_MAX 0.1f

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

    auto* collisionComponent = new CollisionComponent2DAABB(this, size);
    addComponent(collisionComponent);

    InputConfig config = InputConfig();
    config.mouseButtons.push_back(MOUSE_BUTTON_1);
    config.receivesMousePosition = true;
    auto* inputComponent = new InputComponent(config);
    addComponent(inputComponent);

//    std::string text = "0.0";
//    Shader textShader = AssetManager::getInstance()->loadShader("shaders\\text.vert",
//                                                                "shaders\\text.frag",
//                                                                nullptr,
//                                                                text);
//    TextFont textFont = AssetManager::getInstance()->loadTextFont("assets\\arial.ttf",
//                                                                  "arial12", 12);
//    auto* textComponent = new TextComponent(textShader, textFont, text);
//
//    textComponent->setColor(glm::vec3(.25f, .5f, .25f));
//    textComponent->setTransform(glm::vec3(0, -10.f, 0.001f));
//    addComponent(textComponent);

    receivesUpdates = true;
}

void SPCard::update(float deltaTime)
{
    Entity::update(deltaTime);

    auto* inputComponent = getComponent<InputComponent>();

    while(inputComponent->hasMouseEvents())
    {
        MouseEvent event = inputComponent->dequeueMouseEvent();
        switch(event.action) {
            case ACTION_PRESS:
            {
                glm::vec2 cardPosition = this->getWorldTransform().getPosition2();

                if(isTopmostAtPoint(event.position))
                {
                    grabPosition = event.position; // Where the grab started
                    grabOffset = glm::vec2(grabPosition.x - cardPosition.x, grabPosition.y -
                                                                            cardPosition.y); // The offset into the card the grab occurred

                    // Pop the card to the front
                    this->transform.setPosition(glm::vec3(this->transform.getPosition().x,
                                                          this->transform.getPosition().y,
                                                          STACK_MAX));
                }

                break;
            }
            case ACTION_RELEASE:
            {
                grabPosition = NO_GRAB;
                grabOffset = NO_GRAB;

                break;
            }
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

    if(grabPosition == NO_GRAB && grabOffset == NO_GRAB)
        settleCard();

//    std::string depth = std::to_string(this->getTransform()->getPosition().z);
//    this->getComponent<TextComponent>()->setText(depth);

    overlaps.clear();
}

void SPCard::settleCard()
{
    float newZ = STACK_OFFSET;
    for (auto it = overlaps.begin(); it != overlaps.end(); it++)
    {
        if (SPCard *otherCard = dynamic_cast<SPCard *>(it->first))
        {
            if (otherCard->getTransform()->getPosition().z <= this->transform.getPosition().z &&
                otherCard->getTransform()->getPosition().z >= newZ)
            {
                newZ = otherCard->getTransform()->getPosition().z + STACK_OFFSET;
            }
        }
    }

    this->transform.setPosition(glm::vec3(this->transform.getPosition().x,
                                          this->transform.getPosition().y,
                                          newZ));
}

bool SPCard::containsPoint(glm::vec2 point)
{
    glm::vec2 cardPosition = transform.getPosition2();
    return  point.x >= cardPosition.x && point.x <= cardPosition.x + size.x &&
            point.y >= cardPosition.y && point.y <= cardPosition.y + size.y;
}

bool SPCard::isTopmostAtPoint(glm::vec2 point)
{
    bool isTopmost = true;
    if (containsPoint(point))
    {
        for (auto it = overlaps.begin(); it != overlaps.end(); it++)
        {
            if (SPCard *otherCard = dynamic_cast<SPCard *>(it->first))
            {
                if (otherCard->containsPoint(point) &&
                    otherCard->getTransform()->getPosition().z > transform.getPosition().z)
                {
                    isTopmost = false;
                    break;
                }
            }
        }
    }
    else
    {
        isTopmost = false;
    }

    return isTopmost;
}

void SPCard::collisionCallback(ICollisionReceiver *collisionReceiver, glm::vec3 overlap)
{
    overlaps[collisionReceiver] = overlap;
}

