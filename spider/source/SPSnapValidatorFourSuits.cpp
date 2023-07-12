//
// Created by Will on 1/14/2023.
//

#include "SPSnapValidatorFourSuits.h"

#include <algorithm>
#include <iostream>

#include "SPCard.h"
#include "SPPile.h"
#include "SPPointer.h"
#include "EntityManager.h"
#include "EventManager.h"

#include "InputManager.h"
#include "InputComponent.h"

#include "SpriteComponent2D.h"
#include "OptionsManager.h"

#include "AssetManager.h"
#include "TextComponent.h"

#include <iomanip>
#include <sstream>
#include <cstdlib>

bool pilePeeker = true;


SPSnapValidatorFourSuits::SPSnapValidatorFourSuits()
{
    InputConfig config = InputConfig();
    config.mouseButtons.push_back(MOUSE_BUTTON_1);
    config.receivesMousePosition = true;
    config.keys = {KEY_D, KEY_Z};

    auto* inputComponent = new InputComponent(config);
    addComponent(inputComponent);

    if(pilePeeker)
    {
        std::string text = "0.0";
        Shader textShader = AssetManager::getInstance()->loadShader("shaders\\text.vert",
                                                                    "shaders\\text.frag",
                                                                    nullptr,
                                                                    text);
        TextFont textFont = AssetManager::getInstance()->loadTextFont("assets\\arial.ttf",
                                                                      "arial18", 18);
        auto* textComponent = new TextComponent(textShader, textFont, text);
        textComponent->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
        textComponent->setTransform(glm::vec3(0, -10.f, 0.001f));
        addComponent(textComponent);
    }

    OptionsManager::getInstance()->registerReceiver(this);

    this->transform.setPosition(glm::vec3(this->transform.getPosition().x, this->transform.getPosition().y, 0.5f));

    receivesUpdates = true;
}

SPSnapValidatorFourSuits::~SPSnapValidatorFourSuits()
{
    OptionsManager::getInstance()->deregisterReceiver(this);
}

void SPSnapValidatorFourSuits::deal()
{
    for(auto* pile : playPiles)
        if(!pile->getPileChild())
            return;

    for(auto* pile : playPiles)
    {
        SPCard* card = dynamic_cast<SPCard*>(deck->getPileEnd());
        card->raiseToFront();
        card->flip();
        pile->addToPile(card);
    }
    moveList.push_back(MoveEntry(nullptr, nullptr));
}

void SPSnapValidatorFourSuits::undoDeal()
{
    for(std::vector<SPPile*>::reverse_iterator iter = playPiles.rbegin(); iter != playPiles.rend(); iter++)
    {
        SPPile* pile = *iter;
        SPCard* card = dynamic_cast<SPCard*>(pile->getPileEnd());
        card->removeFromPile();
        card->raiseToFront();
        deck->addToPile(card);
    }
}

void SPSnapValidatorFourSuits::initialSetup(Scene *scene)
{
    EntityManager* entityManager = EntityManager::getInstance();

    // Two full decks of cards
    std::vector<SPCard*> cardSet;
    for(int deckNum = 0; deckNum < 8; deckNum++)
    {
//        for(int suit = SPSUIT_MIN; suit < SPSUIT_MAX; suit++)
//        {
            for(int value = SPVALUE_MIN; value < SPVALUE_MAX; value++)
            {
//                SPCard* newCard = new SPCard(glm::vec2(10.f, 10.f), (SPCardSuit) suit, (SPCardValue) value, false, this);
                SPCard* newCard = new SPCard(glm::vec2(10.f, 10.f), SPSUIT_SPADE, (SPCardValue) value, false, this);

                entityManager->registerEntity(scene, newCard);
                uiGrid.registerElement(newCard);
                cardSet.push_back(newCard);
            }
//        }
    }
    std::shuffle(std::begin(cardSet), std::end(cardSet), rng);
    std::shuffle(std::begin(cardSet), std::end(cardSet), rng);
    std::shuffle(std::begin(cardSet), std::end(cardSet), rng);
    std::shuffle(std::begin(cardSet), std::end(cardSet), rng);
    std::shuffle(std::begin(cardSet), std::end(cardSet), rng);
    std::shuffle(std::begin(cardSet), std::end(cardSet), rng);
    std::shuffle(std::begin(cardSet), std::end(cardSet), rng);

    for(int i = 0; i < 10; i++)
    {
        SPPile* pile = new SPPile(glm::vec2(10.f + (i * 55.f), 100.f),
                                  glm::vec3(0.0f, 0.0f, STACK_OFFSET),
                                  glm::vec3(0.0f, 20.0f, STACK_OFFSET),
                                  false, this);
        playPiles.push_back(pile);
        entityManager->registerEntity(scene, pile);
        uiGrid.registerElement(pile);
    }

    // Four face down rows
    for(int i = 0; i < 4; i++)
    {
        for (auto *pile : playPiles)
        {
            pile->addToPile(cardSet.back(), true);
            cardSet.pop_back();
        }
    }

    // Four more cards in the first four piles
    for(int i = 0; i < 4; i++)
    {
        playPiles[i]->addToPile(cardSet.back(), true);
        cardSet.pop_back();
    }

    // One face up card in each pile
    for(auto* pile : playPiles)
    {
        SPCard* card = cardSet.back();
        cardSet.pop_back();
        pile->addToPile(card, true);
        card->flip();
    }

    // 8 "out of play" piles
    for(int i = 0; i < 8; i++)
    {
        SPPile* pile = new SPPile(glm::vec2(100.f + (i * 55.f), 10.f),
                                  glm::vec3(0.0f, 0.0f, STACK_OFFSET),
                                  glm::vec3(0.0f, 0.0f, STACK_OFFSET),
                                  false, this);
        outPiles.push_back(pile);
        entityManager->registerEntity(scene, pile);
    }

    // Put the rest of the cards into the deck
    deck = new SPPile(glm::vec2(10.f, 10.f),
                      glm::vec3(0.0f, 0.0f, STACK_OFFSET),
                      glm::vec3(0.0f, 0.0f, STACK_OFFSET),
                      false, this);

    while(!cardSet.empty())
    {
        deck->addToPile(cardSet.back(), true);
        cardSet.pop_back();
    }

    pointer = new SPPointer(deck->getPileEnd(), glm::vec2(5.0f, 5.0f));
    entityManager->registerEntity(scene, pointer);

    updateLayout();
}

void SPSnapValidatorFourSuits::updateLayout()
{
    glm::vec2 viewportResolution = OptionsManager::getInstance()->getViewportResolution();

    glm::vec2 initialSize = glm::vec2(49.f, 64.f); // TODO: Find a way to not hard code this

    float cardSizeRatio = initialSize.x / initialSize.y;

    float targetWidth = viewportResolution.x / 11; // 11 as in ten piles with pile of width in between
    float gapWidth = targetWidth / 11;

    float targetHeight = targetWidth / cardSizeRatio;

    // PlayPiles
    float currX = gapWidth;
    for(SPPile* playPile : playPiles)
    {
        playPile->getTransform()->setPosition2(glm::vec2(currX, targetHeight + (2 * gapWidth)));

        glm::vec2 prevSize = playPile->getSize();
        float scaleMultiplier = targetWidth/initialSize.x;
        playPile->setSize(glm::vec2(initialSize.x * scaleMultiplier, initialSize.y * scaleMultiplier));

        playPile->setPileOffset(glm::vec3(0.0f, 20.f * targetHeight/initialSize.y, STACK_OFFSET));
        playPile->setInitialPileOffset(playPile->getPileOffset());

        for(SPCard* currCard = dynamic_cast<SPCard*>(playPile->getPileChild());
            currCard != nullptr;
            currCard = dynamic_cast<SPCard*>(currCard->getPileChild()))
        {
            // Card Size
            glm::vec2 prevSize = currCard->getSize();
            float scaleMultiplier = targetWidth/initialSize.x;
            currCard->setSize(glm::vec2(initialSize.x * scaleMultiplier, initialSize.y * scaleMultiplier));

            currCard->setPileOffset(glm::vec3(0.0f, 20.f * targetHeight/initialSize.y, STACK_OFFSET));
            currCard->setInitialPileOffset(currCard->getPileOffset());

            if(currCard->getPileParent() == playPile)
                currCard->snapTo(currCard->getPileParent()->getRootOffset());
            else
                currCard->snapTo(currCard->getPileParent()->getPileOffset());
        }

        currX += targetWidth + gapWidth;
    }

    // OutPiles
    currX = gapWidth * 2 + targetWidth * 1.5;
    float outPileGapWidth = ((viewportResolution.x - currX) - (8 * targetWidth)) / 8;
    for(SPPile* outPile : outPiles)
    {
        outPile->getTransform()->setPosition2(glm::vec2(currX, gapWidth));

        glm::vec2 prevSize = outPile->getSize();
        float scaleMultiplier = targetWidth/initialSize.x;
        outPile->setSize(glm::vec2(initialSize.x * scaleMultiplier, initialSize.y * scaleMultiplier));

        for(SPCard* currCard = dynamic_cast<SPCard*>(outPile->getPileChild());
            currCard != nullptr;
            currCard = dynamic_cast<SPCard*>(currCard->getPileChild()))
        {
            // Card Size
            glm::vec2 prevSize = currCard->getSize();
            float scaleMultiplier = targetWidth/initialSize.x;
            currCard->setSize(glm::vec2(initialSize.x * scaleMultiplier, initialSize.y * scaleMultiplier));
        }

        currX += targetWidth + outPileGapWidth;
    }

    // Deck
    deck->getTransform()->setPosition2(glm::vec2(gapWidth, gapWidth));
    {
        for(SPCard* currCard = dynamic_cast<SPCard*>(deck->getPileChild());
            currCard != nullptr;
            currCard = dynamic_cast<SPCard*>(currCard->getPileChild()))
        {
            // Card Size
            glm::vec2 prevSize = currCard->getSize();
            float scaleMultiplier = targetWidth/initialSize.x;
            currCard->setSize(glm::vec2(initialSize.x * scaleMultiplier, initialSize.y * scaleMultiplier));
        }
    }
}

void SPSnapValidatorFourSuits::reportClick(SPPilable* pilable)
{
    if(pilable->getPileRoot() == deck)
    {
        deal();
    }
}

bool SPSnapValidatorFourSuits::validateGrab(SPPilable *parent, SPPilable *child)
{
    if(SPCard* childCard = dynamic_cast<SPCard*>(child))
        if(childCard->isFaceDown())
            return false;

    if(child->getPileRoot() == deck)
        return false;

    for(auto outPile : outPiles)
        if(child->getPileRoot() == outPile)
            return false;

    // Nothing below this card, legal
    if(child == child->getPileEnd())
        return true;

    if(SPCard* childCard = dynamic_cast<SPCard*>(child))
    {
        SPCardSuit targetSuit = childCard->getSuit();
        SPCardValue targetValue = (SPCardValue) (childCard->getValue() - 1);
        SPPilable* curr = childCard->getPileChild();
        while(curr != nullptr)
        {
            if(SPCard* currCard = dynamic_cast<SPCard*>(curr))
            {
                if(currCard->getSuit() != targetSuit)
                    return false;

                if(currCard->getValue() != targetValue)
                    return false;
            }
            targetValue = (SPCardValue) (targetValue - 1);
            curr = curr->getPileChild();
        }

        // Got to the bottom of the stack with everything in the same suit, legal
        return true;
    }
    else
    {
        return false;
    }
}

void SPSnapValidatorFourSuits::reportGrab(SPPilable *parent, SPPilable *child)
{
    if(parent)
        child->removeFromPile();

    // Pop the card to the front
    child->raiseToFront();

    moveList.push_back(MoveEntry(parent, child));
}

bool SPSnapValidatorFourSuits::validateRelease(SPPilable* parent, SPPilable* child)
{
    if(SPCard* childCard = dynamic_cast<SPCard*>(child))
    {
        if(SPPile* parentPile = dynamic_cast<SPPile*>(parent))
        {
            if(parentPile->incrementing && childCard->getValue() == SPVALUE_ACE)
                return true;
            else if(!parentPile->incrementing)
                return true;
            return false;
        }

        if(SPCard* parentCard = dynamic_cast<SPCard*>(parent))
        {
            if(parentCard->isFaceDown())
                return false;

            if(SPPile* rootPile = dynamic_cast<SPPile*>(parent->getPileRoot()))
            {
                // Never legal to play on an out pile
                for(auto* outPile : outPiles)
                    if(rootPile == outPile)
                        return false;

                // Never legal to play on the deck
                if(rootPile == deck)
                    return false;

                // Remaining piles are play piles
                if(!rootPile->incrementing)
                {
//                    bool suitsAlternate = (parentCard->getSuit() % 2) != (childCard->getSuit() % 2);
                    bool valuesDecrease = parentCard->getValue() == childCard->getValue() + 1;
                    if(valuesDecrease)
                        return true;
                }
            }
        }
    }

    return false;
}

void SPSnapValidatorFourSuits::reportRelease(SPPilable *parent, SPPilable *child)
{
    parent->addToPile(child);

    // Flip face down cards in play when their child is removed
    if(SPCard* oldParentCard = dynamic_cast<SPCard*>(moveList.back().parent))
    {
        // Check that the player didn't put the card back where it came from
        if(parent != oldParentCard && oldParentCard->isFaceDown())
        {
            oldParentCard->flip();
        }

        if(moveList.back().child && moveList.back().parent &&
           moveList.back().child == child &&
           moveList.back().parent == parent)
        {
            // Forget the previous move, player put the card back
            moveList.pop_back();
            return;
        }
    }
}

void SPSnapValidatorFourSuits::undo()
{
    if(moveList.size() <= 0)
        return;

    MoveEntry currentEntry = moveList.back(); moveList.pop_back();

    // If both are nullptr, this move was a deal
    if(currentEntry.parent == nullptr && currentEntry.child == nullptr)
    {
        undoDeal();
        return;
    }

    bool isSuitMove = false;
    if(currentEntry.child->getPileParent())
    {
        // Undo'ing a suited stack to the foundation
        for(SPPile* outPile : outPiles)
            if(currentEntry.child->getPileParent() == outPile)
                isSuitMove = true;
        currentEntry.child->removeFromPile();
    }

    // TODO: Should raiseToFront just be a part of addToPile?
    currentEntry.child->raiseToFront();
    currentEntry.parent->addToPile(currentEntry.child);

    if(SPCard* parentCard = dynamic_cast<SPCard*>(currentEntry.parent))
        if(currentEntry.parentFaceUp != parentCard->isFaceUp())
            parentCard->flip();

    // Ensure all moved cards are receiving updates
    SPCard* currCard = dynamic_cast<SPCard*>(currentEntry.child);
    while(currCard)
    {
        currCard = dynamic_cast<SPCard*>(currCard->getPileChild());
    }

    // Undo again if it's a suit move to break the suited stack
    if(isSuitMove)
        undo();
}

SPCard* SPSnapValidatorFourSuits::getTopmostCardAtPosition(glm::vec2 position)
{
    EntityManager* entityManager = EntityManager::getInstance();
    std::vector<Entity*> entities = entityManager->getEntitiesInScene(entityManager->getSceneForEntity(this));

    for(Entity* entity : entities)
    {
        SPCard* card = dynamic_cast<SPCard*>(entity);
        if(!card)
            continue;

        if(card->isTopmostAtPoint(position))
            return card;
    }

    return nullptr;
}

void SPSnapValidatorFourSuits::update(float deltaTime)
{
    Entity::update(deltaTime);

    static glm::vec2 grabStartPosition = NO_GRAB;
    static glm::vec2 grabOffset = NO_GRAB;
    static SPCard *grabbedCard = nullptr;

    static glm::vec2 lastMousePosition = glm::vec2(0, 0);

    auto* inputComponent = getComponent<InputComponent>();

    while(inputComponent->hasEvents())
    {
        InputEvent event = inputComponent->dequeueEvent();
        if(event.isKeyEvent())
        {
            switch(event.key)
            {
                case KEY_D:
                    if(event.action == ACTION_PRESS)
                        deal();
                    break;
                case KEY_Z:
                    if(event.action == ACTION_PRESS && event.mods == MOD_CONTROL)
                        undo();
                    break;
                default:
                    return;
            }
        }
        else if(event.isMouseEvent())
        {
            switch(event.action)
            {
                case ACTION_PRESS:
                {
                    grabbedCard = getTopmostCardAtPosition(event.position);
                    if (grabbedCard && validateGrab(grabbedCard->getPileParent(), grabbedCard))
                    {
                        glm::vec2 cardPosition = grabbedCard->getWorldTransform().getPosition2();
                        grabStartPosition = event.position;
                        grabOffset = glm::vec2(event.position.x - cardPosition.x,
                                               event.position.y - cardPosition.y);

                        grabbedCard->select();
                        reportGrab(grabbedCard->getPileParent(), grabbedCard);
                    }
                    else
                    {
                        grabbedCard = nullptr;
                    }
                    break;
                }
                case ACTION_RELEASE:
                {
                    if(grabbedCard)
                    {
                        SPPilable* bestPilable = grabbedCard->getClosestOverlap();
                        if(bestPilable)
                            reportRelease(bestPilable, grabbedCard);
                        else
                            undo();
                    }
                    else
                    {
                        if(SPPilable* clickedPilable = getTopmostCardAtPosition(event.position))
                            reportClick(clickedPilable);
                    }

                    grabbedCard = nullptr;
                    break;
                }
                case ACTION_NONE:
                {
                    lastMousePosition = event.position;

                    // We don't care where the mouse is if the card isn't grabbed
                    if (grabbedCard == nullptr)
                        break;

                    // Grabs are done only with mouse 1
                    if (InputManager::getInstance()->getLastMouseState(MOUSE_BUTTON_1) != ACTION_PRESS)
                        break;

                    // A move while the button is pressed is a "drag"
                    // check if we've moved past our minimum drag threshold, then move the card

                    glm::vec2 difference = event.position - grabStartPosition;
                    float differenceLen = glm::length(event.position - grabStartPosition);
                    if (differenceLen > DRAG_THRESHOLD)
                        grabbedCard->getTransform()->setPosition2(glm::vec2(event.position.x - grabOffset.x, event.position.y - grabOffset.y));

                    break;
                }
                default:
                    return;
            }
        }
        else if(event.isGamepadEvent())
        {
            if(event.padButton != GAMEPAD_BUTTON_NONE)
            {
                Entity* newTarget = nullptr;
                if(event.padButton == GAMEPAD_BUTTON_DPAD_UP)
                {
                    if (event.action == ACTION_PRESS)
                    {
                        newTarget = uiGrid.getElementAbove(pointer->getTarget());
                    }
                }
                else if(event.padButton == GAMEPAD_BUTTON_DPAD_DOWN)
                {
                    if (event.action == ACTION_PRESS)
                    {
                        newTarget = uiGrid.getElementBelow(pointer->getTarget());
                    }
                }
                else if(event.padButton == GAMEPAD_BUTTON_DPAD_LEFT)
                {
                    if (event.action == ACTION_PRESS)
                    {
                        newTarget = uiGrid.getElementToLeft(pointer->getTarget());
                    }
                }
                else if(event.padButton == GAMEPAD_BUTTON_DPAD_RIGHT)
                {
                    if (event.action == ACTION_PRESS)
                    {
                        newTarget = uiGrid.getElementToRight(pointer->getTarget());
                    }
                }

                if(newTarget)
                    pointer->setTarget(newTarget);
            }

            if(event.axis != GAMEPAD_AXIS_NONE && abs(event.axisValue) > 0.25f)
                std::cout << "Axis: " << event.axis << " " << event.axisValue << std::endl;

            // Down 13
            // Right 12
            // Up 11
            // Left 14
        }
    }

    if(pilePeeker)
    {
        std::stringstream stream;

        EntityManager* entityManager = EntityManager::getInstance();

        // Get all cards under the mouse cursor
        std::vector<SPCard*> cardsUnderPoint;
        for(Entity* entity : entityManager->getEntitiesInScene(entityManager->getSceneForEntity(this)))
            if(SPCard* card = dynamic_cast<SPCard*>(entity))
                if(card->containsPoint(lastMousePosition))
                    cardsUnderPoint.push_back(card);

        // Sort cards by z depth
        struct compare_predicate
        {
            inline bool operator() (SPCard* card1, SPCard* card2)
            {
                return (card1->getWorldTransform().getPosition().z < card2->getWorldTransform().getPosition().z);
            }
        };
        std::sort(cardsUnderPoint.begin(), cardsUnderPoint.end(), compare_predicate());

        // Print the slot the card is currently occupying
        for(SPCard* card : cardsUnderPoint)
        {
            stream << card->getValue() << " " << card->getWorldTransform().getPosition().z * 10000 << "\n";
        }

        std::string text = stream.str();
        this->getComponent<TextComponent>()->setText(text);
        this->getComponent<TextComponent>()->setTransform(Transform(glm::vec3(lastMousePosition.x, lastMousePosition.y, 0.0f)));
    }
}

void SPSnapValidatorFourSuits::reportAnimationComplete(SPPilable *pilable)
{
    if(pilable->isSelected())
        pilable->deselect();

    for(auto* playPile : playPiles)
        rescalePile(playPile);

    for(auto outPile : outPiles)
        if(pilable->getPileRoot() == outPile)
            return;

    if(pilable->getPileRoot() == deck)
    {
        dynamic_cast<SPCard *>(pilable)->flip();
        return;
    }

    handleCompleteSuitIfFound(pilable);
}

void SPSnapValidatorFourSuits::handleCompleteSuitIfFound(SPPilable *pilable)
{
    if(SPCard* childCard = dynamic_cast<SPCard*>(pilable))
    {
        int currSuit = childCard->getSuit();
        SPCard* topCard = nullptr;

        SPCard* prevCard = dynamic_cast<SPCard*>(childCard->getPileParent());
        while(prevCard)
        {
            if(prevCard->getSuit() != currSuit)
                return;

            if(prevCard->isFaceDown())
                return;

            if(prevCard->getValue() == SPVALUE_KING)
            {
                topCard = prevCard;
                break;
            }

            prevCard = dynamic_cast<SPCard*>(prevCard->getPileParent());
        }

        if(!topCard)
            return;

        // PrevCard is a suited king
        int targetValue = SPVALUE_KING - 1;
        SPCard* currCard = dynamic_cast<SPCard*>(topCard->getPileChild());
        while(currCard)
        {
            if(currCard->getValue() != targetValue)
                return;

            if(currCard->getSuit() != currSuit)
                return;

            // Cards are still moving, wait until everything is settled
            if(currCard != pilable && currCard->hasAnimations())
                return;

            if(currCard->getValue() == SPVALUE_ACE)
                break;

            currCard = dynamic_cast<SPCard*>(currCard->getPileChild());
            targetValue = targetValue - 1;
        }

        if(currCard == nullptr)
            return;

        // If we get here, we have a King through Ace suited
        moveList.push_back(MoveEntry(topCard->getPileParent(), topCard));

        if(SPCard* parentCard = dynamic_cast<SPCard*>(topCard->getPileParent()))
            if(parentCard->isFaceDown())
                parentCard->flip();

        int currOutPile = 0;
        while(outPiles[currOutPile]->getPileChild())
            currOutPile++;

        topCard->removeFromPile();
        topCard->raiseToFront();
        outPiles[currOutPile]->addToPile(topCard);
        currOutPile++;

        // All piles filled, you win!
        // TODO: Make sure all animations are settled before doing this so the final animation isn't lost
        if(currOutPile >= 8)
            EventManager::getInstance()->broadcastEvent(WON_GAME);
    }
}

void SPSnapValidatorFourSuits::rescalePile(SPPilable* pilable)
{
    if(!pilable)
        return;

    if(pilable->getPileRoot() == deck)
        return;

    for(auto* outPile : outPiles)
        if(pilable->getPileRoot() == outPile)
            return;

    glm::vec2 viewportRes = OptionsManager::getInstance()->getViewportResolution();

    // Reset pile to initial offset
    if(pilable->getPileOffset() != pilable->getInitialPileOffset())
    {
        for (SPPilable *currPilable = pilable->getPileRoot();
             currPilable != nullptr; currPilable = currPilable->getPileChild())
        {
            currPilable->setPileOffset(currPilable->getInitialPileOffset());
            if(currPilable != currPilable->getPileRoot() &&
               currPilable->getPileParent() != currPilable->getPileRoot())
                currPilable->snapTo(currPilable->getPileParent()->getPileOffset());
        }
    }

    // Crush pile down if it doesn't fit
    while(pilable->getPileEnd()->getWorldTransform().getPosition().y + pilable->getPileEnd()->getSize().y > viewportRes.y - (pilable->getSize().y / 11))
    {
        for(SPPilable* currPilable = pilable->getPileRoot(); currPilable != nullptr; currPilable = currPilable->getPileChild())
        {
            glm::vec3 offset = currPilable->getPileOffset();
            offset.y -= 1;
            currPilable->setPileOffset(offset);
            if(currPilable != currPilable->getPileRoot() &&
               currPilable->getPileParent() != currPilable->getPileRoot())
                currPilable->snapTo(currPilable->getPileParent()->getPileOffset());
        }
    }
}

void SPSnapValidatorFourSuits::resolutionUpdated(glm::vec2 oldRes, glm::vec2 newRes)
{
    updateLayout();
}