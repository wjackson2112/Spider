//  Copyright (c) 2026 William Jackson
//  Licensed under the MIT License

//
// Created by will on 8/20/25.
//

#include "NSSelectAction.h"
#include "EventManager.h"
#include "InputManager.h"
#include <iostream>
#include "NSGameState.h"
#include "CFDeck.h"
#include "NSEvent.h"
#include "NSGameMode.h"
#include "OptionsManager.h"

bool NSSelectAction::isDraggingCard() {
    return gameMode->gameState->grabbedCard && state == AS_DRAGGING;
}

bool NSSelectAction::validate(CFCard* grabbedCard)
{
    if(CFCard* childCard = dynamic_cast<CFCard*>(grabbedCard))
        if(childCard->isFaceDown())
            return false;

    if(grabbedCard->getPileRoot() == gameMode->gameState->stock)
        return false;

    // std::cout << gameMode->gameState->foundations.size() << std::endl;

    for(auto foundation : gameMode->gameState->foundations)
        if(grabbedCard->getPileRoot() == foundation)
            return false;

    // Nothing below this card, legal
    if(grabbedCard == grabbedCard->getPileEnd())
        return true;

    if(CFCard* childCard = dynamic_cast<CFCard*>(grabbedCard))
    {
        CFCardSuit targetSuit = childCard->getSuit();
        CFCardRank targetValue = (CFCardRank) (childCard->getRank() - 1);
        CFPilable* curr = childCard->getPileChild();
        while(curr != nullptr)
        {
            if(CFCard* currCard = dynamic_cast<CFCard*>(curr))
            {
                if(currCard->getSuit() != targetSuit)
                    return false;

                if(currCard->getRank() != targetValue)
                    return false;
            }
            targetValue = (CFCardRank) (targetValue - 1);
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

void NSSelectAction::press()
{
    glm::vec2 position = InputManager::getCursorPosition();
    // if(InputManager::getInstance()->getInputMode() == IM_GAMEPAD)
    //     position = gameMode->gameState->cursor->getWorldTransform().getPosition2();

    // There's already a selected card, do nothing
    if(gameMode->gameState->grabbedCard)
        return;

    gameMode->gameState->grabbedCard = getTopmostCardAtPosition(position);

    if (gameMode->gameState->grabbedCard && validate(gameMode->gameState->grabbedCard))
    {
        gameMode->gameState->moveList.emplace_back(gameMode->gameState->grabbedCard->getPileParent(), gameMode->gameState->grabbedCard);

        gameMode->gameState->grabbedCard->select();
        if(gameMode->gameState->grabbedCard->getPileParent())
            gameMode->gameState->grabbedCard->removeFromPile();

        // Pop the card to the front
        gameMode->gameState->grabbedCard->raiseToFront();

        glm::vec2 cardPosition = gameMode->gameState->grabbedCard->getWorldTransform().getPosition2();
        gameMode->gameState->grabStartPosition = position;
        gameMode->gameState->grabOffset = glm::vec2(position.x - cardPosition.x,
                               position.y - cardPosition.y);

        // gameState->updateSelectedUIGrid(gameState->grabbedCard);
        // gameState->swapUIGrid();
    }
    else
    {
        gameMode->gameState->grabbedCard = nullptr;
    }

    state = AS_PRESSED;
    // std::cout << "GRABBING" << std::endl;
    // gameMode->gameState->cursor->setTarget(gameMode->gameState->grabbedCard);
}

void NSSelectAction::drag(glm::vec2 position) {
    if(!gameMode->gameState->grabbedCard)
        return;

    glm::vec2 difference = position - gameMode->gameState->grabStartPosition;
    float differenceLen = glm::length(position - gameMode->gameState->grabStartPosition);
    float dragThresold = OptionsManager::getInstance()->getViewportResolution().x / 16;
    if (state == AS_IDLE && differenceLen <= dragThresold)
        return;

    gameMode->gameState->grabbedCard->getTransform()->setPosition2(
            glm::vec2(position.x - gameMode->gameState->grabOffset.x, position.y - gameMode->gameState->grabOffset.y));
    state = AS_DRAGGING;
    // gameMode->gameState->cursor->setTarget(gameMode->gameState->grabbedCard);
    // std::cout << "DRAGGING" << std::endl;
}

void NSSelectAction::drag()
{
    glm::vec2 position = InputManager::getCursorPosition();
    drag(position);
//    if(!gameState->grabbedCard)
//        return;
//
//    glm::vec2 difference = event.position - gameState->grabStartPosition;
//    float differenceLen = glm::length(event.position - gameState->grabStartPosition);
//    if (differenceLen <= DRAG_THRESHOLD)
//        return;
//
//    gameState->grabbedCard->getTransform()->setPosition2(
//            glm::vec2(event.position.x - gameState->grabOffset.x, event.position.y - gameState->grabOffset.y));
//    state = SA_DRAGGING;
//    gameState->cursor->setTarget(gameState->grabbedCard);
//    std::cout << "DRAGGING" << std::endl;
}

void NSSelectAction::release() {
    glm::vec2 position = InputManager::getCursorPosition();

    // If not dragging a card on release, this is a click
    if(!isDraggingCard())
    {
        // If the card clicked on is the top of the deck, send a deal event
        CFCard* topmostCard = getTopmostCardAtPosition(position);
        if(topmostCard && topmostCard->getPileRoot() == gameMode->gameState->stock)
            EventManager::getInstance()->broadcastEvent(Event(NSEvent::EVT_DEAL));

        return;
    }

    // gameState->clearGhostCards();
    CFPilable* bestPilable = gameMode->gameState->grabbedCard->getClosestOverlap();
    if(bestPilable && validateRelease(bestPilable, gameMode->gameState->grabbedCard))
    {
        state = AS_IDLE;

        CFPilable* parent = gameMode->gameState->grabbedCard->getClosestOverlap();
        CFCard* child = gameMode->gameState->grabbedCard;
        gameMode->gameState->grabbedCard = nullptr;

        if(!parent)
        {
            EventManager::getInstance()->broadcastEvent(Event(NSEvent::EVT_CANCEL));
            return;
        }

        //    SPPilable* oldParent = gameState.moveList.back().parent;
        parent->addToPile(child, false);//, child, static_cast<AnimCompleteFunction>(&CFCard::animationComplete)/*, inputMode == IM_GAMEPAD*/);

        //    if(inputMode == IM_GAMEPAD)
        //    {
        //        child->deselect();
        //        handleCompleteSuitIfFound(child);
        //    }

        // Flip face down cards in play when their child is removed
        if(auto oldParentCard = dynamic_cast<CFCard*>(child->getPilePrevParent()))
        {
            // Check that the player didn't put the card back where it came from
            if(parent != oldParentCard && oldParentCard->isFaceDown())
            {
                oldParentCard->flip();
            }

            if(gameMode->gameState->moveList.back().child && gameMode->gameState->moveList.back().parent &&
               gameMode->gameState->moveList.back().child == child &&
               gameMode->gameState->moveList.back().parent == parent)
            {
                // Forget the previous move, player put the card back
                gameMode->gameState->moveList.pop_back();
                return;
            }
        }

        // gameState->unselectedUIGrid.clearBearing();
        // gameState->updateUnselectedUIGrid();
    }

    if(gameMode->gameState->grabbedCard) {
        EventManager::getInstance()->broadcastEvent(Event(NSEvent::EVT_CANCEL));
    }
}

bool NSSelectAction::validateRelease(CFPilable* parent, CFPilable* child)
{
    CFPilable* oldParent = gameMode->gameState->moveList.back().parent;
    if(oldParent == parent)
        return false;

    if(CFCard* childCard = dynamic_cast<CFCard*>(child))
    {
        if(CFPile* parentPile = dynamic_cast<CFPile*>(parent))
        {
            // Never legal to play on a foundation
            for(auto* foundation : gameMode->gameState->foundations)
                if(parentPile == foundation)
                    return false;

            // if(parentPile->incrementing && childCard->getRank() == CF_RANK_ACE)
            //     return true;
            // else if(!parentPile->incrementing)
            //     return true;
            // return false;
            return true;
        }

        if(CFCard* parentCard = dynamic_cast<CFCard*>(parent))
        {
            if(parentCard->isFaceDown())
                return false;

            if(CFPile* pileRoot = dynamic_cast<CFPile*>(parent->getPileRoot()))
            {
                // Never legal to play on the deck
                if(pileRoot == reinterpret_cast<CFPile *>(gameMode->gameState->stock))
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

void NSSelectAction::eventCallback(Event event) {
    switch(event)
    {
        case NSEvent::EVT_PRESS:
            press();
            return;
        case NSEvent::EVT_DRAG:
            drag();
            return;
        case NSEvent::EVT_RELEASE:
            release();
            return;
        default:
            break;
    }

    Entity* newTarget = nullptr;

    switch(event)
    {
        case Event::EVT_SWITCH_INPUT_MODE:
            EventManager::getInstance()->broadcastEvent(Event(NSEvent::EVT_CANCEL));
            // if(InputManager::getInstance()->getInputMode() == IM_GAMEPAD)
            //     gameMode->gameState->cursor->setTarget(gameMode->gameState->tableaus[0]->getPileEnd());
            return;
        // case SPEvent::EVT_LEFT:
        //     newTarget = gameState->activeUIGrid->getElementToLeft(gameState->cursor->getTarget());
        //     break;
        // case SPEvent::EVT_RIGHT:
        //     newTarget = gameState->activeUIGrid->getElementToRight(gameState->cursor->getTarget());
        //     break;
        // case SPEvent::EVT_UP:
        //     newTarget = gameState->activeUIGrid->getElementAbove(gameState->cursor->getTarget());
        //     break;
        // case SPEvent::EVT_DOWN:
        //     newTarget = gameState->activeUIGrid->getElementBelow(gameState->cursor->getTarget());
        //     break;
        default:
            return;
    }

    // InputManager::setCursorPosition(gameMode->gameState->cursor->getWorldTransform().getPosition2().x,
    //                                 gameMode->gameState->cursor->getWorldTransform().getPosition2().y);

    if(!newTarget)
        return;

    // gameMode->gameState->cursor->setTarget(newTarget);

    // Move the grabbed card and point to it
    // if (gameMode->gameState->grabbedCard)
    // {
    //     glm::vec2 cursorPosition = gameMode->gameState->cursor->getWorldTransform().getPosition2();
    //     auto offset = glm::vec3(0);
    //     if(auto* pilableTarget = dynamic_cast<CFPilable*>(newTarget))
    //     {
    //         if (pilableTarget == pilableTarget->getPileRoot())
    //             offset = pilableTarget->getRootOffset();
    //         else
    //             offset = pilableTarget->getPileOffset();
    //     }
    //
    //     glm::vec2 newPosition = cursorPosition + glm::vec2(offset.x, offset.y);
    //
    //     drag(newPosition);
    //     gameMode->gameState->cursor->setTarget(gameMode->gameState->grabbedCard);
    // }
}
