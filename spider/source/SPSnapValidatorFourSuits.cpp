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
//#include "InputComponent.h"

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
#include "SPCancelAction.h"
#include "SPDealAction.h"
#include "SPUndoAction.h"
//#include "SPDragAction.h"
//#include "SPReleaseAction.h"

//#define PILEPEEKER


SPSnapValidatorFourSuits::SPSnapValidatorFourSuits()
{
    gameState.gameMode = this;

//    InputConfig config = InputConfig();
//    auto* inputComponent = new InputComponent(config);
//    addComponent(inputComponent);

    gameState.selectAction = new SPSelectAction(&gameState);
    gameState.cancelAction = new SPCancelAction(&gameState);
    gameState.dealAction = new SPDealAction(&gameState);
    gameState.undoAction = new SPUndoAction(&gameState);

    // TODO: Move all these input bindings to some external class with a save/load serialization option
    // Window Inputs
    InputManager::getInstance()->addBinding(KEY_ESCAPE, ACTION_PRESS, Event(Event::EVT_QUIT_GAME));
    InputManager::getInstance()->addBinding(KEY_F1, ACTION_PRESS, Event(Event::EVT_WIREFRAME_MODE));

    // Directional Input
    InputManager::getInstance()->addBinding(MOUSE_BUTTON_NONE, ACTION_NONE, Event(SPEvent::EVT_DRAG));
    InputManager::getInstance()->addBinding(GAMEPAD_BUTTON_DPAD_UP, ACTION_PRESS, Event(SPEvent::EVT_UP));
    InputManager::getInstance()->addBinding(GAMEPAD_BUTTON_DPAD_DOWN, ACTION_PRESS, Event(SPEvent::EVT_DOWN));
    InputManager::getInstance()->addBinding(GAMEPAD_BUTTON_DPAD_LEFT, ACTION_PRESS, Event(SPEvent::EVT_LEFT));
    InputManager::getInstance()->addBinding(GAMEPAD_BUTTON_DPAD_RIGHT, ACTION_PRESS, Event(SPEvent::EVT_RIGHT));

    // Press
    InputManager::getInstance()->addBinding(MOUSE_BUTTON_1, ACTION_PRESS, Event(SPEvent::EVT_PRESS));
    InputManager::getInstance()->addBinding(GAMEPAD_BUTTON_A, ACTION_PRESS, Event(SPEvent::EVT_PRESS));

    // Release
    InputManager::getInstance()->addBinding(MOUSE_BUTTON_1, ACTION_RELEASE, Event(SPEvent::EVT_RELEASE));
    InputManager::getInstance()->addBinding(GAMEPAD_BUTTON_A, ACTION_RELEASE, Event(SPEvent::EVT_RELEASE));

    // Cancel
    InputManager::getInstance()->addBinding(MOUSE_BUTTON_2, ACTION_RELEASE, Event(SPEvent::EVT_CANCEL));
    InputManager::getInstance()->addBinding(GAMEPAD_BUTTON_B, ACTION_RELEASE, Event(SPEvent::EVT_CANCEL));

    // Deal
    InputManager::getInstance()->addBinding(KEY_D, ACTION_PRESS, Event(SPEvent::EVT_DEAL));
    InputManager::getInstance()->addBinding(GAMEPAD_BUTTON_Y, ACTION_PRESS, Event(SPEvent::EVT_DEAL));

    // Undo
    InputManager::getInstance()->addBinding(KEY_Z, ACTION_PRESS, Event(SPEvent::EVT_UNDO), MOD_CONTROL);
    InputManager::getInstance()->addBinding(GAMEPAD_BUTTON_X, ACTION_PRESS, Event(SPEvent::EVT_UNDO));


#ifdef PILEPEEKER
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
#endif

    OptionsManager::getInstance()->registerReceiver(this);

    this->transform.setPosition(glm::vec3(this->transform.getPosition().x, this->transform.getPosition().y, 0.5f));

    shouldUpdate = true;

    gameState.unselectedUIGrid.setPriorityX(UIGD_PRIORITIZE_AXIS);
    gameState.unselectedUIGrid.setPriorityY(UIGD_PRIORITIZE_ALIGNMENT);
    gameState.unselectedUIGrid.wrapX = true;
    gameState.unselectedUIGrid.wrapY = true;

    gameState.selectedUIGrid.setPriorityX(UIGD_PRIORITIZE_AXIS);
    gameState.selectedUIGrid.setPriorityY(UIGD_PRIORITIZE_ALIGNMENT);
    gameState.selectedUIGrid.wrapX = true;
    gameState.selectedUIGrid.wrapY = true;

    gameState.activeUIGrid = &gameState.unselectedUIGrid;
}

SPSnapValidatorFourSuits::~SPSnapValidatorFourSuits()
{
    delete(gameState.selectAction);
    delete(gameState.dealAction);
    OptionsManager::getInstance()->deregisterReceiver(this);
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
            for(int rank = CF_RANK_MIN; rank <= CF_RANK_MAX; rank++)
            {
//                SPCard* newCard = new SPCard(glm::vec2(10.f, 10.f), (CFCardSuit) suit, (CFCardRank) value, false, this);
//                SPCard* newCard = new SPCard();

//                entityManager->registerEntity(scene, newCard);
                auto* newCard = scene->addEntity<SPCard>(glm::vec2(10.f, 10.f), CF_SUIT_SPADE, static_cast<CFCardRank>(rank), false, &gameState);
                cardSet.push_back(newCard);
            }
//        }
    }

    // std::shuffle(std::begin(cardSet), std::end(cardSet), rng);
    // std::shuffle(std::begin(cardSet), std::end(cardSet), rng);
    // std::shuffle(std::begin(cardSet), std::end(cardSet), rng);
    // std::shuffle(std::begin(cardSet), std::end(cardSet), rng);
    // std::shuffle(std::begin(cardSet), std::end(cardSet), rng);
    // std::shuffle(std::begin(cardSet), std::end(cardSet), rng);
    // std::shuffle(std::begin(cardSet), std::end(cardSet), rng);

    for(int i = 0; i < 10; i++)
    {
        auto* tableau = scene->addEntity<SPPile>(glm::vec2(10.f + (i * 55.f), 100.f),
                                  glm::vec3(0.0f, 0.0f, STACK_OFFSET),
                                  glm::vec3(0.0f, 20.0f, STACK_OFFSET));
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
                                  glm::vec3(0.0f, 0.0f, STACK_OFFSET));
        gameState.foundations.push_back(foundation);
//        entityManager->registerEntity(scene, pile);
    }

    gameState.stock = new CFDeck(glm::vec2(10.f, 10.f));

    while(!cardSet.empty())
    {
        gameState.stock->addToPile(cardSet.back(), true);
        cardSet.pop_back();
    }

    gameState.cursor = scene->addEntity<SPCursor>(gameState.tableaus[0]->getPileEnd(), glm::vec2(5.0f, 5.0f));
    gameState.cursor->enable();
//    entityManager->registerEntity(scene, gameState.cursor);

    updateLayout();
    gameState.updateUnselectedUIGrid();
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

bool SPGameState::validateRelease(CFPilable* parent, CFPilable* child)
{
    CFPilable* oldParent = moveList.back().parent;
    if(oldParent == parent)
        return false;

    if(SPCard* childCard = dynamic_cast<SPCard*>(child))
    {
        if(SPPile* parentPile = dynamic_cast<SPPile*>(parent))
        {
            // Never legal to play on a foundation
            for(auto* foundation : foundations)
                if(parentPile == foundation)
                    return false;

            // if(parentPile->incrementing && childCard->getRank() == CF_RANK_ACE)
            //     return true;
            // else if(!parentPile->incrementing)
            //     return true;
            // return false;
            return true;
        }

        if(SPCard* parentCard = dynamic_cast<SPCard*>(parent))
        {
            if(parentCard->isFaceDown())
                return false;

            if(CFPile* pileRoot = dynamic_cast<CFPile*>(parent->getPileRoot()))
            {
                // Never legal to play on the deck
                if(pileRoot == stock)
                    return false;

                // Remaining piles are play piles
                // if(!pileRoot->incrementing)
                // {
//                    bool suitsAlternate = (parentCard->getSuit() % 2) != (childCard->getSuit() % 2);
                    bool valuesDecrease = parentCard->getRank() == childCard->getRank() + 1;
                    if(valuesDecrease)
                        return true;
                // }
            }
        }
    }

    return false;
}

//void SPSnapValidatorFourSuits::update(float deltaTime)
//{
//    Entity::update(deltaTime);
//
////    static glm::vec2 grabStartPosition = NO_GRAB;
////    static glm::vec2 grabOffset = NO_GRAB;
////    static SPCard *grabbedCard = nullptr;
//
//#ifdef PILEPEEKER
//    static glm::vec2 lastMousePosition = glm::vec2(0, 0);
//#endif
//
//    auto* inputComponent = getComponent<InputComponent>();
//
//    while(inputComponent->hasEvents())
//    {
//        InputEvent event = inputComponent->dequeueEvent();
//        if(event.isMouseEvent())
//        {
////            if(InputManager::getInstance()->getInputMode() != IM_MKB)
////            {
////                if(gameState.grabbedCard)
////                {
////                    EventManager::getInstance()->broadcastEvent(Event(SPEvent::EVT_CANCEL));
////                }
//////                gameState.inputMode = IM_MKB;
////            }
//
//            switch(event.action)
//            {
//                case ACTION_NONE:
//                {
//#ifdef PILEPEEKER
//                    lastMousePosition = event.position;
//#endif
//                    break;
//                }
//                default:
//                    return;
//            }
//        }
//        if(event.isGamepadEvent())
//        {
////            if(gameState.inputMode != IM_GAMEPAD)
////            {
//////                gameState.cursor->enable();
//////                gameState.cursor->setTarget(gameState.tableaus[0]->getPileEnd());
////                if(gameState.grabbedCard)
////                {
////                    EventManager::getInstance()->broadcastEvent(Event(SPEvent::EVT_CANCEL));
////                }
////                gameState.inputMode = IM_GAMEPAD;
////            }
//
//            if(event.padButton != GAMEPAD_BUTTON_NONE)
//            {
//                Entity* newTarget = nullptr;
//                glm::vec2 cursorPosition = InputManager::getCursorPosition();
//                switch(event.padButton)
//                {
////                    case GAMEPAD_BUTTON_DPAD_UP: {
////                        if (event.action == ACTION_PRESS)
////                        {
////                            newTarget = gameState.activeUIGrid->getElementAbove(gameState.cursor->getTarget());
////                            if(!newTarget)
////                                continue;
////
////                            gameState.cursor->setTarget(newTarget);
////                            cursorPosition = gameState.cursor->getWorldTransform().getPosition2();
////
////
////
////                            glm::vec3 offset = glm::vec3(0);
////                            if(SPPilable* pilableTarget = dynamic_cast<SPPilable*>(newTarget))
////                            {
////                                if (pilableTarget == pilableTarget->getPileRoot())
////                                    offset = pilableTarget->getRootOffset();
////                                else
////                                    offset = pilableTarget->getPileOffset();
////                            }
////
////                            event.position = cursorPosition + glm::vec2(offset.x, offset.y);
////
////                            if (gameState.grabbedCard)
////                            {
////                                gameState.selectAction->drag(event.position);
//////                                gameState.grabbedCard->getTransform()->setPosition2(glm::vec2(cursorPosition.x + offset.x - gameState.select->grabOffset.x,
//////                                                                                    cursorPosition.y + offset.y - gameState.select->grabOffset.y));
////                                gameState.cursor->setTarget(gameState.grabbedCard);
////                            }
////                        }
////                        break;
////                    }
////                    case GAMEPAD_BUTTON_DPAD_DOWN:
////                    {
////                        if (event.action == ACTION_PRESS)
////                        {
////                            newTarget = gameState.activeUIGrid->getElementBelow(gameState.cursor->getTarget());
////                            if(!newTarget)
////                                continue;
////
////                            gameState.cursor->setTarget(newTarget);
////                            cursorPosition = gameState.cursor->getWorldTransform().getPosition2();
////
////                            glm::vec3 offset = glm::vec3(0);
////                            if(SPPilable* pilableTarget = dynamic_cast<SPPilable*>(newTarget))
////                            {
////                                if (pilableTarget == pilableTarget->getPileRoot())
////                                    offset = pilableTarget->getRootOffset();
////                                else
////                                    offset = pilableTarget->getPileOffset();
////                            }
////
////                            event.position = cursorPosition + glm::vec2(offset.x, offset.y);
////
////                            if (gameState.grabbedCard)
////                            {
////                                gameState.selectAction->drag(event.position);
//////                                gameState.grabbedCard->getTransform()->setPosition2(glm::vec2(cursorPosition.x + offset.x - gameState.select->grabOffset.x,
//////                                                                                    cursorPosition.y + offset.y - gameState.select->grabOffset.y));
////                                gameState.cursor->setTarget(gameState.grabbedCard);
////                            }
////                        }
////                        break;
////                    }
////                    case GAMEPAD_BUTTON_DPAD_LEFT:
////                    {
////                        if (event.action == ACTION_PRESS)
////                        {
////                            newTarget = gameState.activeUIGrid->getElementToLeft(gameState.cursor->getTarget());
////                            if(!newTarget)
////                                continue;
////
////                            gameState.cursor->setTarget(newTarget);
////                            cursorPosition = gameState.cursor->getWorldTransform().getPosition2();
////                            InputManager::setCursorPosition(cursorPosition);
////
////                            glm::vec3 offset = glm::vec3(0);
////                            if(SPPilable* pilableTarget = dynamic_cast<SPPilable*>(newTarget))
////                            {
////                                if (pilableTarget == pilableTarget->getPileRoot())
////                                    offset = pilableTarget->getRootOffset();
////                                else
////                                    offset = pilableTarget->getPileOffset();
////                            }
////
////                            event.position = cursorPosition + glm::vec2(offset.x, offset.y);
////
////                            if (gameState.grabbedCard)
////                            {
////                                gameState.selectAction->drag(event.position);
//////                                gameState.grabbedCard->getTransform()->setPosition2(glm::vec2(cursorPosition.x + offset.x - gameState.select->grabOffset.x,
//////                                                                                    cursorPosition.y + offset.y - gameState.select->grabOffset.y));
////                                gameState.cursor->setTarget(gameState.grabbedCard);
////                            }
////                        }
////                        break;
////                    }
////                    case GAMEPAD_BUTTON_DPAD_RIGHT:
////                    {
////                        if (event.action == ACTION_PRESS) {
////                            Entity* oldTarget = gameState.cursor->getTarget();
////                            if(oldTarget)
////                                newTarget = gameState.activeUIGrid->getElementToRight(gameState.cursor->getTarget());
////                            else
////                                newTarget = gameState.tableaus[0]->getPileEnd();
////
////                            if(!newTarget)
////                                continue;
////
////                            gameState.cursor->setTarget(newTarget);
////                            cursorPosition = gameState.cursor->getWorldTransform().getPosition2();
////                            InputManager::setCursorPosition(cursorPosition);
////
////
////                            glm::vec3 offset = glm::vec3(0);
////                            if(SPPilable* pilableTarget = dynamic_cast<SPPilable*>(newTarget))
////                            {
////                                if (pilableTarget == pilableTarget->getPileRoot())
////                                    offset = pilableTarget->getRootOffset();
////                                else
////                                    offset = pilableTarget->getPileOffset();
////                            }
////
////                            event.position = cursorPosition + glm::vec2(offset.x, offset.y);
////
////                            if (gameState.grabbedCard)
////                            {
////                                gameState.selectAction->drag(event.position);
////
//////                                gameState.grabbedCard->getTransform()->setPosition2(glm::vec2(cursorPosition.x + offset.x - gameState.select->grabOffset.x,
//////                                                                                    cursorPosition.y + offset.y - gameState.select->grabOffset.y));
//////                                gameState.cursor->setTarget(gameState.grabbedCard);
////                            }
////                        }
////                        break;
////                    }
////                    case GAMEPAD_BUTTON_A:
////                    {
////                        if (event.action == ACTION_PRESS) {
////                            gameState.selectAction->press();
////                            break;
////                        }
////                        else if(event.action == ACTION_RELEASE)
////                        {
////                            gameState.selectAction->release();
////                            break;
////                        }
////                        break;
////                    }
////                    case GAMEPAD_BUTTON_B:
////                    {
////                        if (event.action == ACTION_RELEASE) {
////                            gameState.cancelAction->release(event);
////                        }
////                        break;
////                    }
////                    case GAMEPAD_BUTTON_Y:
////                    {
////                        if (event.action == ACTION_PRESS) {
////                            gameState.dealAction->press(event);
////                        }
////                        break;
////                    }
////                    case GAMEPAD_BUTTON_X:
////                    {
////                        if (event.action == ACTION_PRESS) {
////                            gameState.undoAction->press(event);
////                        }
////                        break;
////                    }
//                }
//
//                InputManager::setCursorPosition(cursorPosition);
//            }
//
//            if(event.axis != GAMEPAD_AXIS_NONE && abs(event.axisValue) > 0.25f)
//                std::cout << "Axis: " << event.axis << " " << event.axisValue << std::endl;
//
//
//
//            // Down 13
//            // Right 12
//            // Up 11
//            // Left 14
//        }
//
//        if(gameState.grabbedCard)
//            gameState.updateGhostCards(gameState.grabbedCard);
//    }
//
//#ifdef PILEPEEKER
//    std::stringstream stream;
//
////    EntityManager* entityManager = EntityManager::getInstance();
//
//    // Get all cards under the mouse cursor
//    std::vector<SPCard*> cardsUnderPoint;
//    std::vector<Entity*> entities = owningScene->getEntities();
//    for(Entity* entity : entities)
//        if(SPCard* card = dynamic_cast<SPCard*>(entity))
//            if(card->containsPoint(lastMousePosition))
//                cardsUnderPoint.push_back(card);
//
//    // Sort cards by z depth
//    struct compare_predicate
//    {
//        inline bool operator() (SPCard* card1, SPCard* card2)
//        {
//            return (card1->getWorldTransform().getPosition().z < card2->getWorldTransform().getPosition().z);
//        }
//    };
//    std::sort(cardsUnderPoint.begin(), cardsUnderPoint.end(), compare_predicate());
//
//    // Print the slot the card is currently occupying
//    for(SPCard* card : cardsUnderPoint)
//    {
//        stream << card->getValue() << " " << card->getWorldTransform().getPosition().x << " " << card->getWorldTransform().getPosition().y << "\n";
//    }
//
//    std::string text = stream.str();
//    this->getComponent<TextComponent>()->setText(text);
//    this->getComponent<TextComponent>()->setTransform(Transform(glm::vec3(lastMousePosition.x, lastMousePosition.y, 0.0f)));
//# endif
//}

void SPGameState::animationCompleteWithId(std::string identifier, Entity *entity)
{
    gameMode->animationCompleteWithId(identifier, entity);
}

void SPSnapValidatorFourSuits::animationCompleteWithId(std::string identifier, Entity *entity)
{
    CFPilable* pilable = dynamic_cast<CFPilable*>(entity);

    if(pilable->isSelected())
        pilable->deselect();

    // TODO: Rescale just the two affected piles
    for(auto* tableau : gameState.tableaus)
        rescalePile(tableau);

    // All piles filled, you win!
    // TODO: Maybe it's worth checking if the parent is an outpile too to optimize this?
    if(dynamic_cast<SPCard*>(pilable)->getRank() == CF_RANK_KING)
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
            EventManager::getInstance()->broadcastEvent(Event(Event::EVT_WON_GAME));
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

void SPSnapValidatorFourSuits::handleCompleteSuitIfFound(CFPilable *pilable)
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

            if(prevCard->getRank() == CF_RANK_KING)
            {
                topCard = prevCard;
                break;
            }

            prevCard = dynamic_cast<SPCard*>(prevCard->getPileParent());
        }

        if(!topCard)
            return;

        // PrevCard is a suited king
        int targetValue = CF_RANK_KING - 1;
        SPCard* currCard = dynamic_cast<SPCard*>(topCard->getPileChild());
        while(currCard)
        {
            if(currCard->getRank() != targetValue)
                return;

            if(currCard->getSuit() != currSuit)
                return;

            // Cards are still moving, wait until everything is settled
            if(currCard != pilable && currCard->hasUnfinishedAnimations())
                return;

            if(currCard->getRank() == CF_RANK_ACE)
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

        gameState.cursor->setTarget(topCard->getPileParent());

        topCard->removeFromPile();
        topCard->raiseToFront();
        gameState.foundations[currFoundation]->addToPile(topCard, false, this);
        currFoundation++;

        gameState.updateUnselectedUIGrid();
    }
}

void SPSnapValidatorFourSuits::rescalePile(CFPilable* pilable)
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
        for (CFPilable *currPilable = pilable->getPileRoot();
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
        for(CFPilable* currPilable = pilable->getPileRoot(); currPilable != nullptr; currPilable = currPilable->getPileChild())
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

void SPGameState::updateUnselectedUIGrid()
{
    unselectedUIGrid.deregisterAllElements();

    for(SPPile* tableau : tableaus)
    {
        if(tableau->getPileChild() == nullptr)
        {
            unselectedUIGrid.registerElement(tableau);
            continue;
        }

        for(CFPilable* pilable = tableau->getPileEnd(); pilable != nullptr; pilable = pilable->getPileParent())
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

void SPGameState::updateSelectedUIGrid(SPCard* selectedCard)
{
    selectedUIGrid.deregisterAllElements();

    selectedUIGrid.registerElement(selectedCard->getPilePrevParent());

    // for(SPPile* tableau : tableaus)
    // {
    //     if(validateRelease(tableau->getPileEnd(), selectedCard))
    //         selectedUIGrid.registerElement(tableau->getPileEnd());
    // }
}

void SPGameState::swapUIGrid()
{
    if(activeUIGrid == &unselectedUIGrid)
        activeUIGrid = &selectedUIGrid;
    else
        activeUIGrid = &unselectedUIGrid;
}

void SPGameState::updateGhostCards(SPCard* selectedCard)
{
    for(SPPile* tableau : tableaus)
    {
        if(tableau->getPileEnd() == selectedCard)
            continue;

        if(validateRelease(tableau->getPileEnd(), selectedCard))
        {
            for(CFPilable* currPilable = selectedCard; currPilable != nullptr; currPilable = currPilable->getPileChild())
            {
                auto* currCard = dynamic_cast<SPCard*>(currPilable);

                auto* ghostCard = owningScene->addEntity<SPCard>(glm::vec2(0,0), currCard->getSuit(), currCard->getRank(), true, this);
                auto* ghostSpriteComponent = ghostCard->getComponent<SpriteComponent2D>();
                auto* ghostAnimationComponent = ghostCard->getComponent<AnimationComponent>();

                tableau->addToPile(ghostCard, true);
                ghostSpriteComponent->setColor4(glm::vec4(0.5f, 0.75f, 0.5f, 0.0f));
                ghostAnimationComponent->addAndStart<ColorAnimation>(ghostCard, glm::vec4(0.5f, 0.75f, 0.5f, 0.25f), 0.1f);
                ghostCards.push_back(ghostCard);
            }
        }
    }

    gameMode->updateLayout();
}

void SPGameState::clearGhostCards()
{
    if(ghostCards.empty())
        return;

    for(auto it = ghostCards.begin(); it != ghostCards.end(); it++)
    {
        (*it)->removeFromPile();
        glm::vec3 ghostPosition = (*it)->getTransform()->getPosition();
        (*it)->getTransform()->setPosition(glm::vec3(ghostPosition.x, ghostPosition.y, ghostPosition.z - STACK_OFFSET/2));
        (*it)->getComponent<AnimationComponent>()->skipAll();

//        auto* ghostSpriteComponent = (*it)->getComponent<SpriteComponent2D>();
        auto* ghostAnimationComponent = (*it)->getComponent<AnimationComponent>();
        ghostAnimationComponent->addAndStart<ColorAnimation>((*it), glm::vec4(0.5f, 0.75f, 0.5f, 0.0f), 0.1f, this, (AnimCompleteFunction) &SPGameState::ghostCardClearComplete);
    }
}

void SPGameState::ghostCardClearComplete(Entity* card)
{
//    owningScene->removeEntity(card);
    card->shouldBeDestroyed = true;
    ghostCards.erase(std::remove(ghostCards.begin(), ghostCards.end(), card), ghostCards.end());
}

