//
// Created by Will on 1/14/2023.
//

#include "SPSnapValidatorFourSuits.h"

#include <algorithm>
#include <iostream>

#include "SPCard.h"
#include "SPPile.h"
#include "SPCursor.h"
//#include "EntityManager.h"
#include "EventManager.h"

#include "InputManager.h"
#include "InputComponent.h"

#include "SpriteComponent2D.h"
#include "AnimationComponent.h"
#include "ColorAnimation.h"
#include "OptionsManager.h"

#include "AssetManager.h"
#include "TextComponent.h"

#include <iomanip>
#include <sstream>
#include <cstdlib>

#include "SPSelectAction.h"

bool pilePeeker = false;


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
        Shader textShader = AssetManager::getInstance()->loadShader("shaders/text.vert",
                                                                    "shaders/text.frag",
                                                                    nullptr,
                                                                    text);
        TextFont textFont = AssetManager::getInstance()->loadTextFont("assets/arial.ttf",
                                                                      "arial18", 18);
        auto* textComponent = new TextComponent(textShader, textFont, text);
        textComponent->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
        textComponent->setTransform(glm::vec3(0, -10.f, 0.001f));
        addComponent(textComponent);
    }

    OptionsManager::getInstance()->registerReceiver(this);

    this->transform.setPosition(glm::vec3(this->transform.getPosition().x, this->transform.getPosition().y, 0.5f));

    receivesUpdates = true;

    unselectedUIGrid.setPriorityX(UIGD_PRIORITIZE_AXIS);
    unselectedUIGrid.setPriorityY(UIGD_PRIORITIZE_ALIGNMENT);
    unselectedUIGrid.wrapX = true;
    unselectedUIGrid.wrapY = true;

    selectedUIGrid.setPriorityX(UIGD_PRIORITIZE_AXIS);
    selectedUIGrid.setPriorityY(UIGD_PRIORITIZE_ALIGNMENT);
    selectedUIGrid.wrapX = true;
    selectedUIGrid.wrapY = true;

    activeUIGrid = &unselectedUIGrid;
}

SPSnapValidatorFourSuits::~SPSnapValidatorFourSuits()
{
    OptionsManager::getInstance()->deregisterReceiver(this);
}

void SPSnapValidatorFourSuits::deal()
{
    // No dealing if any of the tableaus are empty
    for(auto* tableau : gameState.tableaus)
        if(!tableau->getPileChild())
            return;

    for(auto* tableau : gameState.tableaus)
    {
        SPCard* card = dynamic_cast<SPCard*>(gameState.stock->getPileEnd());
        card->raiseToFront();
        card->flip();
        tableau->addToPile(card);
    }
    gameState.moveList.emplace_back(MoveEntry(nullptr, nullptr));

    unselectedUIGrid.clearBearing();
    updateUnselectedUIGrid();
}

void SPSnapValidatorFourSuits::undoDeal()
{
    for(std::vector<SPPile*>::reverse_iterator iter = gameState.tableaus.rbegin(); iter != gameState.tableaus.rend(); iter++)
    {
        SPPile* tableau = *iter;
        SPCard* card = dynamic_cast<SPCard*>(tableau->getPileEnd());
        card->removeFromPile();
        card->raiseToFront();
        gameState.stock->addToPile(card);

        if(gamepadCursor->getTarget() == card)
            gamepadCursor->setTarget(activeUIGrid->getElementBelow(card));
    }

    updateUnselectedUIGrid();
}

void SPSnapValidatorFourSuits::initialSetup(Scene *scene)
{
//    EntityManager* entityManager = EntityManager::getInstance();

    gameState.owningScene = owningScene;

    // Two full decks of cards
    std::vector<SPCard*> cardSet;
    for(int deckNum = 0; deckNum < 8; deckNum++)
    {
//        for(int suit = SPSUIT_MIN; suit <= SPSUIT_MAX; suit++)
//        {
            for(int value = SPVALUE_MIN; value <= SPVALUE_MAX; value++)
            {
//                SPCard* newCard = new SPCard(glm::vec2(10.f, 10.f), (SPCardSuit) suit, (SPCardValue) value, false, this);
//                SPCard* newCard = new SPCard();

//                entityManager->registerEntity(scene, newCard);
                auto* newCard = scene->addEntity<SPCard>(glm::vec2(10.f, 10.f), SPSUIT_SPADE, (SPCardValue) value, false, this);
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
        auto* tableau = scene->addEntity<SPPile>(glm::vec2(10.f + (i * 55.f), 100.f),
                                  glm::vec3(0.0f, 0.0f, STACK_OFFSET),
                                  glm::vec3(0.0f, 20.0f, STACK_OFFSET),
                                  false, this);
        gameState.tableaus.push_back(tableau);
//        entityManager->registerEntity(scene, pile);
    }

    // Four face down rows
    for(int i = 0; i < 4; i++)
    {
        for (auto *tableau : gameState.tableaus)
        {
            tableau->addToPile(cardSet.back(), true);
            cardSet.pop_back();
        }
    }

    // Four more cards in the first four piles
    for(int i = 0; i < 4; i++)
    {
        gameState.tableaus[i]->addToPile(cardSet.back(), true);
        cardSet.pop_back();
    }

    // One face up card in each pile
    for(auto* tableau : gameState.tableaus)
    {
        SPCard* card = cardSet.back();
        cardSet.pop_back();
        tableau->addToPile(card, true);
        card->flip();
    }

    // 8 "out of play" piles
    for(int i = 0; i < 8; i++)
    {
        auto* foundation = scene->addEntity<SPPile>(glm::vec2(100.f + (i * 55.f), 10.f),
                                  glm::vec3(0.0f, 0.0f, STACK_OFFSET),
                                  glm::vec3(0.0f, 0.0f, STACK_OFFSET),
                                  false, this);
        gameState.foundations.push_back(foundation);
//        entityManager->registerEntity(scene, pile);
    }

    // Put the rest of the cards into the deck
    gameState.stock = new SPPile(glm::vec2(10.f, 10.f),
                                 glm::vec3(0.0f, 0.0f, STACK_OFFSET),
                                 glm::vec3(0.0f, 0.0f, STACK_OFFSET),
                                 false, this);

    while(!cardSet.empty())
    {
        gameState.stock->addToPile(cardSet.back(), true);
        cardSet.pop_back();
    }

    gamepadCursor = scene->addEntity<SPCursor>(gameState.tableaus[0]->getPileEnd(), glm::vec2(5.0f, 5.0f));
//    entityManager->registerEntity(scene, gamepadCursor);

    updateLayout();
    updateUnselectedUIGrid();
}

void SPSnapValidatorFourSuits::updateLayout()
{
    glm::vec2 viewportResolution = OptionsManager::getInstance()->getViewportResolution();

    glm::vec2 initialSize = glm::vec2(49.f, 64.f); // TODO: Find a way to not hard code this

    float cardSizeRatio = initialSize.x / initialSize.y;

    float targetWidth = viewportResolution.x / 11; // 11 as in ten piles with pile of width in between
    float gapWidth = targetWidth / 11;

    float targetHeight = targetWidth / cardSizeRatio;

    // Tableaus
    float currX = gapWidth;
    for(SPPile* tableau : gameState.tableaus)
    {
        tableau->getTransform()->setPosition2(glm::vec2(currX, targetHeight + (2 * gapWidth)));

        glm::vec2 prevSize = tableau->getSize();
        float scaleMultiplier = targetWidth/initialSize.x;
        tableau->setSize(glm::vec2(initialSize.x * scaleMultiplier, initialSize.y * scaleMultiplier));

        tableau->setPileOffset(glm::vec3(0.0f, 20.f * targetHeight/initialSize.y, STACK_OFFSET));
        tableau->setInitialPileOffset(tableau->getPileOffset());

        for(SPCard* currCard = dynamic_cast<SPCard*>(tableau->getPileChild());
            currCard != nullptr;
            currCard = dynamic_cast<SPCard*>(currCard->getPileChild()))
        {
            // Card Size
            glm::vec2 prevSize = currCard->getSize();
            float scaleMultiplier = targetWidth/initialSize.x;
            currCard->setSize(glm::vec2(initialSize.x * scaleMultiplier, initialSize.y * scaleMultiplier));

            currCard->setPileOffset(glm::vec3(0.0f, 20.f * targetHeight/initialSize.y, STACK_OFFSET));
            currCard->setInitialPileOffset(currCard->getPileOffset());

            if(currCard->getPileParent() == tableau)
                currCard->snapTo(currCard->getPileParent()->getRootOffset());
            else
                currCard->snapTo(currCard->getPileParent()->getPileOffset());
        }

        currX += targetWidth + gapWidth;
    }

    // Foundations
    currX = gapWidth * 2 + targetWidth * 1.5;
    float outPileGapWidth = ((viewportResolution.x - currX) - (8 * targetWidth)) / 8;
    for(SPPile* foundation : gameState.foundations)
    {
        foundation->getTransform()->setPosition2(glm::vec2(currX, gapWidth));

        glm::vec2 prevSize = foundation->getSize();
        float scaleMultiplier = targetWidth/initialSize.x;
        foundation->setSize(glm::vec2(initialSize.x * scaleMultiplier, initialSize.y * scaleMultiplier));

        for(SPCard* currCard = dynamic_cast<SPCard*>(foundation->getPileChild());
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

    // Stock
    gameState.stock->getTransform()->setPosition2(glm::vec2(gapWidth, gapWidth));
    {
        for(SPCard* currCard = dynamic_cast<SPCard*>(gameState.stock->getPileChild());
            currCard != nullptr;
            currCard = dynamic_cast<SPCard*>(currCard->getPileChild()))
        {
            // Card Size
            glm::vec2 prevSize = currCard->getSize();
            float scaleMultiplier = targetWidth/initialSize.x;
            currCard->setSize(glm::vec2(initialSize.x * scaleMultiplier, initialSize.y * scaleMultiplier));
        }
    }

    for(SPPile* tableau : gameState.tableaus)
    {
        rescalePile(tableau);
    }
}

void SPSnapValidatorFourSuits::reportClick(SPPilable* pilable)
{
    if(pilable->getPileRoot() == gameState.stock)
    {
        deal();
    }
}

//bool SPSnapValidatorFourSuits::validateGrab(SPPilable *parent, SPPilable *child)
//{
//    if(SPCard* childCard = dynamic_cast<SPCard*>(child))
//        if(childCard->isFaceDown())
//            return false;
//
//    if(child->getPileRoot() == gameState.stock)
//        return false;
//
//    for(auto foundation : gameState.foundations)
//        if(child->getPileRoot() == foundation)
//            return false;
//
//    // Nothing below this card, legal
//    if(child == child->getPileEnd())
//        return true;
//
//    if(SPCard* childCard = dynamic_cast<SPCard*>(child))
//    {
//        SPCardSuit targetSuit = childCard->getSuit();
//        SPCardValue targetValue = (SPCardValue) (childCard->getValue() - 1);
//        SPPilable* curr = childCard->getPileChild();
//        while(curr != nullptr)
//        {
//            if(SPCard* currCard = dynamic_cast<SPCard*>(curr))
//            {
//                if(currCard->getSuit() != targetSuit)
//                    return false;
//
//                if(currCard->getValue() != targetValue)
//                    return false;
//            }
//            targetValue = (SPCardValue) (targetValue - 1);
//            curr = curr->getPileChild();
//        }
//
//        // Got to the bottom of the stack with everything in the same suit, legal
//        return true;
//    }
//    else
//    {
//        return false;
//    }
//}
//
//void SPSnapValidatorFourSuits::reportGrab(SPPilable *parent, SPPilable *child)
//{
//    if(parent)
//        child->removeFromPile();
//
//    // Pop the card to the front
//    child->raiseToFront();
//
//    gameState.moveList.push_back(MoveEntry(parent, child));
//}

bool SPSnapValidatorFourSuits::validateRelease(SPPilable* parent, SPPilable* child)
{
    SPPilable* oldParent = gameState.moveList.back().parent;
    if(oldParent == parent)
        return false;

    if(SPCard* childCard = dynamic_cast<SPCard*>(child))
    {
        if(SPPile* parentPile = dynamic_cast<SPPile*>(parent))
        {
            // Never legal to play on a foundation
            for(auto* foundation : gameState.foundations)
                if(parentPile == foundation)
                    return false;

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

            if(SPPile* pileRoot = dynamic_cast<SPPile*>(parent->getPileRoot()))
            {
                // Never legal to play on the deck
                if(pileRoot == gameState.stock)
                    return false;

                // Remaining piles are play piles
                if(!pileRoot->incrementing)
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
    SPPilable* oldParent = gameState.moveList.back().parent;
    parent->addToPile(child, inputMode == IM_GAMEPAD);

    if(inputMode == IM_GAMEPAD)
    {
        child->deselect();
        handleCompleteSuitIfFound(child);
    }

    // Flip face down cards in play when their child is removed
    if(auto oldParentCard = dynamic_cast<SPCard*>(oldParent))
    {
        // Check that the player didn't put the card back where it came from
        if(parent != oldParentCard && oldParentCard->isFaceDown())
        {
            oldParentCard->flip();
        }

        if(gameState.moveList.back().child && gameState.moveList.back().parent &&
                gameState.moveList.back().child == child &&
                gameState.moveList.back().parent == parent)
        {
            // Forget the previous move, player put the card back
            gameState.moveList.pop_back();
            return;
        }
    }

    unselectedUIGrid.clearBearing();
    updateUnselectedUIGrid();
}

void SPSnapValidatorFourSuits::undo()
{
    if(gameState.moveList.size() <= 0)
        return;

    MoveEntry currentEntry = gameState.moveList.back(); gameState.moveList.pop_back();

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
        for(SPPile* foundation : gameState.foundations)
            if(currentEntry.child->getPileParent() == foundation)
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
    else
        updateUnselectedUIGrid();

    if(inputMode == IM_GAMEPAD)
        currentEntry.child->deselect();
}

SPCard* SPSnapValidatorFourSuits::getTopmostCardAtPosition(glm::vec2 position)
{
//    EntityManager* entityManager = EntityManager::getInstance();
//    std::vector<Entity*> entities = entityManager->getEntitiesInScene(entityManager->getSceneForEntity(this));
    std::vector<Entity*> entities = owningScene->getEntities();
    for(Entity* entity : entities)
    {
        auto* card = dynamic_cast<SPCard*>(entity);
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

//    static glm::vec2 grabStartPosition = NO_GRAB;
//    static glm::vec2 grabOffset = NO_GRAB;
//    static SPCard *grabbedCard = nullptr;

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
            if(inputMode != IM_MKB)
            {
                gamepadCursor->disable();
                if(gameState.grabbedCard)
                {
                    undo();
                    swapUIGrid();
                }
                inputMode = IM_MKB;
            }

            switch(event.action)
            {
                case ACTION_PRESS:
                {
                    SPSelectAction(&gameState).execute(event);
//                    gameState.grabbedCard = getTopmostCardAtPosition(event.position);
//                    if (gameState.grabbedCard && validateGrab(gameState.grabbedCard->getPileParent(), gameState.grabbedCard))
//                    {
//                        glm::vec2 cardPosition = gameState.grabbedCard->getWorldTransform().getPosition2();
//                        grabStartPosition = event.position;
//                        grabOffset = glm::vec2(event.position.x - cardPosition.x,
//                                               event.position.y - cardPosition.y);
//
//                        gameState.grabbedCard->select();
//                        reportGrab(gameState.grabbedCard->getPileParent(), gameState.grabbedCard);
//                    }
//                    else
//                    {
//                        gameState.grabbedCard = nullptr;
//                    }
                    break;
                }
                case ACTION_RELEASE:
                {
                    if(gameState.grabbedCard)
                    {
                        SPPilable* bestPilable = gameState.grabbedCard->getClosestOverlap();
                        if(bestPilable)
                            reportRelease(bestPilable, gameState.grabbedCard);
                        else
                            undo();
                    }
                    else
                    {
                        if(SPPilable* clickedPilable = getTopmostCardAtPosition(event.position))
                            reportClick(clickedPilable);
                    }

                    gameState.grabbedCard = nullptr;
                    break;
                }
                case ACTION_NONE:
                {
                    lastMousePosition = event.position;

                    // We don't care where the mouse is if the card isn't grabbed
                    if (gameState.grabbedCard == nullptr)
                        break;

                    // Grabs are done only with mouse 1
                    if (InputManager::getInstance()->getLastMouseState(MOUSE_BUTTON_1) != ACTION_PRESS)
                        break;

                    // A move while the button is pressed is a "drag"
                    // check if we've moved past our minimum drag threshold, then move the card

                    glm::vec2 difference = event.position - gameState.grabStartPosition;
                    float differenceLen = glm::length(event.position - gameState.grabStartPosition);
                    if (differenceLen > DRAG_THRESHOLD)
                        gameState.grabbedCard->getTransform()->setPosition2(glm::vec2(event.position.x - gameState.grabOffset.x, event.position.y - gameState.grabOffset.y));

                    break;
                }
                default:
                    return;
            }
        }
        else if(event.isGamepadEvent())
        {
            if(event.padButton != GAMEPAD_BUTTON_NONE && inputMode == IM_GAMEPAD)
            {
                Entity* newTarget = nullptr;
                glm::vec2 cursorPosition = InputManager::getCursorPosition();
                switch(event.padButton)
                {
                    case GAMEPAD_BUTTON_DPAD_UP: {
                        if (event.action == ACTION_PRESS)
                        {
                            newTarget = activeUIGrid->getElementAbove(gamepadCursor->getTarget());
                            if(!newTarget)
                                continue;

                            gamepadCursor->setTarget(newTarget);
                            cursorPosition = gamepadCursor->getWorldTransform().getPosition2();



                            glm::vec3 offset = glm::vec3(0);
                            if(SPPilable* pilableTarget = dynamic_cast<SPPilable*>(newTarget))
                            {
                                if (pilableTarget == pilableTarget->getPileRoot())
                                    offset = pilableTarget->getRootOffset();
                                else
                                    offset = pilableTarget->getPileOffset();
                            }

                            if (gameState.grabbedCard)
                            {
                                gameState.grabbedCard->getTransform()->setPosition2(glm::vec2(cursorPosition.x + offset.x - gameState.grabOffset.x,
                                                                                    cursorPosition.y + offset.y - gameState.grabOffset.y));
                                gamepadCursor->setTarget(gameState.grabbedCard);
                            }
                        }
                        break;
                    }
                    case GAMEPAD_BUTTON_DPAD_DOWN:
                    {
                        if (event.action == ACTION_PRESS)
                        {
                            newTarget = activeUIGrid->getElementBelow(gamepadCursor->getTarget());
                            if(!newTarget)
                                continue;

                            gamepadCursor->setTarget(newTarget);
                            cursorPosition = gamepadCursor->getWorldTransform().getPosition2();

                            glm::vec3 offset = glm::vec3(0);
                            if(SPPilable* pilableTarget = dynamic_cast<SPPilable*>(newTarget))
                            {
                                if (pilableTarget == pilableTarget->getPileRoot())
                                    offset = pilableTarget->getRootOffset();
                                else
                                    offset = pilableTarget->getPileOffset();
                            }

                            if (gameState.grabbedCard)
                            {
                                gameState.grabbedCard->getTransform()->setPosition2(glm::vec2(cursorPosition.x + offset.x - gameState.grabOffset.x,
                                                                                    cursorPosition.y + offset.y - gameState.grabOffset.y));
                                gamepadCursor->setTarget(gameState.grabbedCard);
                            }
                        }
                        break;
                    }
                    case GAMEPAD_BUTTON_DPAD_LEFT:
                    {
                        if (event.action == ACTION_PRESS)
                        {
                            newTarget = activeUIGrid->getElementToLeft(gamepadCursor->getTarget());
                            if(!newTarget)
                                continue;

                            gamepadCursor->setTarget(newTarget);
                            cursorPosition = gamepadCursor->getWorldTransform().getPosition2();
                            InputManager::setCursorPosition(cursorPosition);

                            glm::vec3 offset = glm::vec3(0);
                            if(SPPilable* pilableTarget = dynamic_cast<SPPilable*>(newTarget))
                            {
                                if (pilableTarget == pilableTarget->getPileRoot())
                                    offset = pilableTarget->getRootOffset();
                                else
                                    offset = pilableTarget->getPileOffset();
                            }

                            if (gameState.grabbedCard)
                            {
                                gameState.grabbedCard->getTransform()->setPosition2(glm::vec2(cursorPosition.x + offset.x - gameState.grabOffset.x,
                                                                                    cursorPosition.y + offset.y - gameState.grabOffset.y));
                                gamepadCursor->setTarget(gameState.grabbedCard);
                            }
                        }
                        break;
                    }
                    case GAMEPAD_BUTTON_DPAD_RIGHT:
                    {
                        if (event.action == ACTION_PRESS) {
                            newTarget = activeUIGrid->getElementToRight(gamepadCursor->getTarget());
                            if(!newTarget)
                                continue;

                            gamepadCursor->setTarget(newTarget);
                            cursorPosition = gamepadCursor->getWorldTransform().getPosition2();
                            InputManager::setCursorPosition(cursorPosition);

                            glm::vec3 offset = glm::vec3(0);
                            if(SPPilable* pilableTarget = dynamic_cast<SPPilable*>(newTarget))
                            {
                                if (pilableTarget == pilableTarget->getPileRoot())
                                    offset = pilableTarget->getRootOffset();
                                else
                                    offset = pilableTarget->getPileOffset();
                            }

                            if (gameState.grabbedCard)
                            {
                                gameState.grabbedCard->getTransform()->setPosition2(glm::vec2(cursorPosition.x + offset.x - gameState.grabOffset.x,
                                                                                    cursorPosition.y + offset.y - gameState.grabOffset.y));
                                gamepadCursor->setTarget(gameState.grabbedCard);
                            }
                        }
                        break;
                    }
                    case GAMEPAD_BUTTON_A:
                    {
                        if (event.action == ACTION_PRESS) {
                            if (gameState.grabbedCard)
                            {
                                clearGhostCards();

                                SPPilable* bestPilable = gameState.grabbedCard->getClosestOverlap();
                                if(bestPilable)
                                    reportRelease(bestPilable, gameState.grabbedCard);
                                else
                                    undo();

                                gameState.grabbedCard = nullptr;
                                swapUIGrid();
                            }
                            else
                            {
                                SPSelectAction(&gameState).execute(event);
                                if(gameState.grabbedCard)
                                {
                                    updateSelectedUIGrid(gameState.grabbedCard);
                                    swapUIGrid();
                                }
                            }
                            break;
                        }
                    }
                    case GAMEPAD_BUTTON_B:
                    {
                        if (event.action == ACTION_PRESS) {
                            if (gameState.grabbedCard)
                            {
                                clearGhostCards();
                                undo();
                                gameState.grabbedCard = nullptr;
                                swapUIGrid();
                            }
                        }
                        break;
                    }
                    case GAMEPAD_BUTTON_Y:
                    {
                        if (event.action == ACTION_PRESS) {
                            if (!gameState.grabbedCard)
                            {
                                deal();
                            }
                        }
                        break;
                    }
                    case GAMEPAD_BUTTON_X:
                    {
                        if (event.action == ACTION_PRESS) {
                            if (!gameState.grabbedCard)
                            {
                                undo();
                            }
                        }
                        break;
                    }
                }

                InputManager::setCursorPosition(cursorPosition);

                if(gameState.grabbedCard)
                    updateGhostCards(gameState.grabbedCard);
            }
            else if(inputMode != IM_GAMEPAD)
            {
                gamepadCursor->enable();
                gamepadCursor->setTarget(gameState.tableaus[0]->getPileEnd());
                inputMode = IM_GAMEPAD;
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

//        EntityManager* entityManager = EntityManager::getInstance();

        // Get all cards under the mouse cursor
        std::vector<SPCard*> cardsUnderPoint;
        std::vector<Entity*> entities = owningScene->getEntities();
        for(Entity* entity : entities)
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
            stream << card->getValue() << " " << card->getWorldTransform().getPosition().x << " " << card->getWorldTransform().getPosition().y << "\n";
        }

        std::string text = stream.str();
        this->getComponent<TextComponent>()->setText(text);
        this->getComponent<TextComponent>()->setTransform(Transform(glm::vec3(lastMousePosition.x, lastMousePosition.y, 0.0f)));
    }
}

void SPSnapValidatorFourSuits::reportAnimationComplete(SPPilable *pilable)
{
//    SPSnapValidatorFourSuits::alignPile(pilable);

    if(pilable->isSelected())
        pilable->deselect();

    for(auto* tableau : gameState.tableaus)
        rescalePile(tableau);

    // All piles filled, you win!
    // TODO: Maybe it's worth checking if the parent is an outpile too to optimize this?
    if(dynamic_cast<SPCard*>(pilable)->getValue() == SPVALUE_KING)
    {
        bool winDetected = gameState.foundations.size() == 8;

        for(int currFoundation = 0; currFoundation < gameState.foundations.size(); currFoundation++)
        {
            if (!gameState.foundations[currFoundation]->getPileChild()) {
                winDetected = false;
                break;
            }
        }

        if(winDetected)
            EventManager::getInstance()->broadcastEvent(WON_GAME);
    }

    for(auto foundation : gameState.foundations)
        if(pilable->getPileRoot() == foundation)
            return;

    if(pilable->getPileRoot() == gameState.stock)
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
            if(currCard != pilable && currCard->hasUnfinishedAnimations())
                return;

            if(currCard->getValue() == SPVALUE_ACE)
                break;

            currCard = dynamic_cast<SPCard*>(currCard->getPileChild());
            targetValue = targetValue - 1;
        }

        if(currCard == nullptr)
            return;

        // If we get here, we have a King through Ace suited
        gameState.moveList.push_back(MoveEntry(topCard->getPileParent(), topCard));

        if(SPCard* parentCard = dynamic_cast<SPCard*>(topCard->getPileParent()))
            if(parentCard->isFaceDown())
                parentCard->flip();

        int currFoundation = 0;
        while(gameState.foundations[currFoundation]->getPileChild())
            currFoundation++;

        gamepadCursor->setTarget(topCard->getPileParent());

        topCard->removeFromPile();
        topCard->raiseToFront();
        gameState.foundations[currFoundation]->addToPile(topCard);
        currFoundation++;

        updateUnselectedUIGrid();
    }
}

void SPSnapValidatorFourSuits::rescalePile(SPPilable* pilable)
{
    if(!pilable)
        return;

    if(pilable->getPileRoot() == gameState.stock)
        return;

    for(auto* foundation : gameState.foundations)
        if(pilable->getPileRoot() == foundation)
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

void SPSnapValidatorFourSuits::alignPile(SPPilable* pilable)
{
    SPPilable *currPilable = pilable->getPileRoot();
    currPilable->getPileChild()->getTransform()->setPosition(currPilable->getRootOffset());
    currPilable = currPilable->getPileChild();

    for (;currPilable->getPileChild() != nullptr; currPilable = currPilable->getPileChild())
    {
        currPilable->getPileChild()->getTransform()->setPosition(currPilable->getPileOffset());
    }
}

void SPSnapValidatorFourSuits::resolutionUpdated(glm::vec2 oldRes, glm::vec2 newRes)
{
    updateLayout();
}

void SPSnapValidatorFourSuits::updateUnselectedUIGrid()
{
    unselectedUIGrid.deregisterAllElements();

    for(SPPile* tableau : gameState.tableaus)
    {
        if(tableau->getPileChild() == nullptr)
        {
            unselectedUIGrid.registerElement(tableau);
            continue;
        }

        for(SPPilable* pilable = tableau->getPileEnd(); pilable != nullptr; pilable = pilable->getPileParent())
        {
            if(SPCard* card = dynamic_cast<SPCard*>(pilable))
            {
                if(card->isFaceUp())
                {
                    unselectedUIGrid.registerElement(card);
                }
            }
        }
    }
}

void SPSnapValidatorFourSuits::updateSelectedUIGrid(SPCard* selectedCard)
{
    selectedUIGrid.deregisterAllElements();

    selectedUIGrid.registerElement(selectedCard->getPilePrevParent());

    for(SPPile* tableau : gameState.tableaus)
    {
        if(validateRelease(tableau->getPileEnd(), selectedCard))
            selectedUIGrid.registerElement(tableau->getPileEnd());
    }
}

void SPSnapValidatorFourSuits::swapUIGrid()
{
    if(activeUIGrid == &unselectedUIGrid)
        activeUIGrid = &selectedUIGrid;
    else
        activeUIGrid = &unselectedUIGrid;
}

void SPSnapValidatorFourSuits::updateGhostCards(SPCard* selectedCard)
{
    for(SPPile* tableau : gameState.tableaus)
    {
        if(tableau->getPileEnd() == selectedCard)
            continue;

        if(validateRelease(tableau->getPileEnd(), selectedCard))
        {
            for(SPPilable* currPilable = selectedCard; currPilable != nullptr; currPilable = currPilable->getPileChild())
            {
                auto* currCard = dynamic_cast<SPCard*>(currPilable);

//                auto* ghostCard = new SPCard(glm::vec2(0,0), currCard->getSuit(), currCard->getValue(), true, this);
                auto* ghostCard = owningScene->addEntity<SPCard>(glm::vec2(0,0), currCard->getSuit(), currCard->getValue(), true, this);
                auto* ghostSpriteComponent = ghostCard->getComponent<SpriteComponent2D>();
                auto* ghostAnimationComponent = ghostCard->getComponent<AnimationComponent>();

                tableau->addToPile(ghostCard, true);
//                EntityManager::getInstance()->registerEntity(EntityManager::getInstance()->getSceneForEntity(selectedCard), ghostCard);
                ghostSpriteComponent->setColor4(glm::vec4(0.5f, 0.75f, 0.5f, 0.0f));
                ghostAnimationComponent->addAndStart<ColorAnimation>(ghostCard, glm::vec4(0.5f, 0.75f, 0.5f, 0.25f), 0.1f);
                ghostCards.push_back(ghostCard);
            }
        }
    }

    updateLayout();
}

void SPSnapValidatorFourSuits::clearGhostCards()
{
    for(auto it = ghostCards.begin(); it != ghostCards.end(); it++)
    {
        (*it)->removeFromPile();
        glm::vec3 ghostPosition = (*it)->getTransform()->getPosition();
        (*it)->getTransform()->setPosition(glm::vec3(ghostPosition.x, ghostPosition.y, ghostPosition.z - STACK_OFFSET/2));
        (*it)->getComponent<AnimationComponent>()->skipAll();

        auto* ghostSpriteComponent = (*it)->getComponent<SpriteComponent2D>();
        auto* ghostAnimationComponent = (*it)->getComponent<AnimationComponent>();
        ghostAnimationComponent->addAndStart<ColorAnimation>((*it), glm::vec4(0.5f, 0.75f, 0.5f, 0.0f), 0.1f, this, (AnimCompleteFunction) &SPSnapValidatorFourSuits::ghostCardClearComplete);
    }
}

void SPSnapValidatorFourSuits::ghostCardClearComplete(Entity* card)
{
//    EntityManager::getInstance()->deregisterEntity(card);
    ghostCards.erase(std::remove(ghostCards.begin(), ghostCards.end(), card), ghostCards.end());
}

