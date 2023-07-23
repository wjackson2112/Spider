//
// Created by Will on 1/9/2023.
//

#include "SPCard.h"

#include <iostream>

#include "AssetManager.h"
#include "EntityManager.h"
//#include "TextComponent.h"
#include "SpriteSheetComponent2D.h"
#include "CollisionComponent2DAABB.h"
#include "AnimationComponent.h"
#include "TransformAnimation.h"
#include "OptionsManager.h"

#include "EventManager.h"

#define CARD_BACK_X_INDEX 13

SPCard::SPCard(glm::vec2 position, SPCardSuit suit, SPCardValue value, bool faceUp, SPSnapValidator* validator)
: suit(suit)
, value(value)
, color(SPBACK_BLUE)
, faceUp(faceUp)
, validator(validator)
, size(49.f, 64.f)
{
    this->transform.setPosition2(position);

    Shader shader = AssetManager::getInstance()->loadShader("shaders\\sprite.vert",
                                                            "shaders\\sprite.frag",
                                                            nullptr,
                                                            "sprite");
    // Original deck asset is from here: https://www.codeproject.com/Articles/1187548/Video-Poker
    Texture2D texture = AssetManager::getInstance()->loadTexture("assets\\deck+backs.png", true, "deck");

    SpriteSheetComponent2D* spriteComponent;
    if(faceUp)
        spriteComponent = new SpriteSheetComponent2D(shader, texture, size, glm::vec2(14, 4), glm::vec2(value, suit));
    else
        spriteComponent = new SpriteSheetComponent2D(shader, texture, size, glm::vec2(14, 4), glm::vec2(CARD_BACK_X_INDEX, color));
    addComponent(spriteComponent);

    auto* animationComponent = new AnimationComponent();
    addComponent(animationComponent);

//    std::string text = "0.0";
//    Shader textShader = AssetManager::getInstance()->loadShader("shaders\\text.vert",
//                                                                "shaders\\text.frag",
//                                                                nullptr,
//                                                                text);
//    TextFont textFont = AssetManager::getInstance()->loadTextFont("assets\\arial.ttf",
//                                                                  "arial12", 14);
//    auto* textComponent = new TextComponent(textShader, textFont, text);
//    textComponent->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
//    textComponent->setTransform(glm::vec3(0, -10.f, 0.001f));
//    addComponent(textComponent);

    receivesUpdates = faceUp;
}

SPCard::~SPCard()
{
    removeFromPile();
    if(pileChild)
        pileChild->removeFromPile();
}

SPPilable* SPCard::getClosestOverlap()
{
    SPPilable *bestPilable = nullptr;
    float bestArea = 0.0f;

    EntityManager* entityManager = EntityManager::getInstance();
    std::vector<Entity*> entities = entityManager->getEntitiesInScene(entityManager->getSceneForEntity(this));
    for(Entity* entity : entities)
    {
        if(SPPilable* newPilable = dynamic_cast<SPPilable*>(entity))
        {
            // We're already piled with this card
            if(isInPile(newPilable))
                continue;

            SPPilable* newParent = newPilable->getPileEnd();

            glm::vec2 thisPosition = getWorldTransform().getPosition();
            glm::vec2 thisSize = this->size;
            glm::vec2 otherPosition = newPilable->getWorldTransform().getPosition();
            glm::vec2 otherSize = newPilable->getSize();

            bool xCollided = thisPosition.x + thisSize.x > otherPosition.x &&
                             otherPosition.x + otherSize.x > thisPosition.x;

            bool yCollided = thisPosition.y + thisSize.y > otherPosition.y &&
                             otherPosition.y + otherSize.y > thisPosition.y;

            if(!xCollided || !yCollided)
                continue;

            glm::vec3 start = glm::vec3(0.0f, 0.0f, 0.0f);
            glm::vec3 end = glm::vec3(0.0f, 0.0f, 0.0f);

            if (thisPosition.x < otherPosition.x)
                start.x = otherPosition.x;
            else
                start.x = thisPosition.x;

            if (thisPosition.x + thisSize.x < otherPosition.x + otherSize.x)
                end.x = thisPosition.x + thisSize.x;
            else
                end.x = otherPosition.x + otherSize.x;

            if (thisPosition.y < otherPosition.y)
                start.y = otherPosition.y;
            else
                start.y = thisPosition.y;

            if (thisPosition.y + thisSize.y < otherPosition.y + otherSize.y)
                end.y = thisPosition.y + thisSize.y;
            else
                end.y = otherPosition.y + otherSize.y;

            glm::vec3 overlap = (end - start);

            float newArea = overlap.x * overlap.y;

            // TODO: This is gross and shouldn't be here
            if (newArea > bestArea && validator->validateRelease(newParent, this)) {
                bestPilable = newPilable;
                bestArea = newArea;
            }
        }
    }

    return bestPilable;
}

bool SPCard::containsPoint(glm::vec2 point)
{
    glm::vec2 cardPosition = getWorldTransform().getPosition2();
    return  point.x >= cardPosition.x && point.x <= cardPosition.x + size.x &&
            point.y >= cardPosition.y && point.y <= cardPosition.y + size.y;
}

bool SPCard::isTopmostAtPoint(glm::vec2 point)
{
    if(!containsPoint(point))
        return false;

    EntityManager* entityManager = EntityManager::getInstance();
    std::vector<Entity*> entities = entityManager->getEntitiesInScene(entityManager->getSceneForEntity(this));
    for(Entity* entity : entities)
    {
        if (SPCard *otherCard = dynamic_cast<SPCard *>(entity))
        {
            if (otherCard->containsPoint(point) &&
                otherCard->getWorldTransform().getPosition().z > getWorldTransform().getPosition().z)
            {
                return false;
            }
        }
    }

    return true;
}

void SPCard::flip()
{
    faceUp = !faceUp;

    // Start receiving updates when card turned face up
    // NOTE: Face down disable is handled in the animation callback
    if(isFaceUp())
        receivesUpdates = true;

    if(faceUp)
        getComponent<SpriteSheetComponent2D>()->setSprite(glm::vec2(value, suit));
    else
        getComponent<SpriteSheetComponent2D>()->setSprite(glm::vec2(CARD_BACK_X_INDEX, color));
};

bool SPCard::hasAnimations()
{
    return getComponent<AnimationComponent>()->hasAnimations();
}

void SPCard::select()
{
    SPPilable::select();
//    getComponent<SpriteComponent2D>()->setColor3(glm::vec3(0.75f, 1.0f, .75f));
}

void SPCard::deselect()
{
    SPPilable::deselect();
//    getComponent<SpriteComponent2D>()->setColor3(glm::vec3(1.0f, 1.0f, 1.0f));
}

void SPCard::moveTo(glm::vec3 target)
{
    glm::vec3 translation = target - transform.getPosition();

    auto* animComp = getComponent<AnimationComponent>();

    // Determine where the card is already going, so we can just add enough animation
    // to get to the new target without overshooting
    for(auto transformAnim : animComp->getAnimations<TransformAnimation>())
        if(!transformAnim.hasFinished())
            translation -= transformAnim.getRemainingTranslation();

    Transform targetTransform = this->transform;
    targetTransform.translate(translation);

    animComp->addAndStart(new TransformAnimation(&transform, targetTransform, 0.3f, this));
}

void SPCard::setSize(glm::vec2 size)
{
    this->size = size;
    getComponent<SpriteSheetComponent2D>()->setSize(size);
}

void SPCard::animationComplete()
{
    // Turn off updates for face down cards after they're all settled
    if(!getComponent<AnimationComponent>()->hasAnimations() && isFaceDown())
        receivesUpdates = false;

    validator->reportAnimationComplete(this);
}