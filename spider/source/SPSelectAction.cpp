//
// Created by Will on 4/7/2024.
//

#include "SPSelectAction.h"
#include "EventManager.h"
#include "InputManager.h"
#include <iostream>

bool SPSelectAction::isDraggingCard() {
    return gameState->selectAction->gameState->grabbedCard && gameState->selectAction->state == SA_DRAGGING;
}

bool SPSelectAction::validate(SPCard* grabbedCard)
{
    if(SPCard* childCard = dynamic_cast<SPCard*>(grabbedCard))
        if(childCard->isFaceDown())
            return false;

    if(grabbedCard->getPileRoot() == gameState->stock)
        return false;

    std::cout << gameState->foundations.size() << std::endl;

    for(auto foundation : gameState->foundations)
        if(grabbedCard->getPileRoot() == foundation)
            return false;

    // Nothing below this card, legal
    if(grabbedCard == grabbedCard->getPileEnd())
        return true;

    if(SPCard* childCard = dynamic_cast<SPCard*>(grabbedCard))
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

void SPSelectAction::press()
{
    glm::vec2 position = InputManager::getCursorPosition();
    if(InputManager::getInstance()->getInputMode() == IM_GAMEPAD)
        position = gameState->cursor->getWorldTransform().getPosition2();

    // There's already a selected card, do nothing
    if(gameState->grabbedCard)
        return;

    gameState->grabbedCard = getTopmostCardAtPosition(position);

    if (gameState->grabbedCard && validate(gameState->grabbedCard))
    {
        gameState->moveList.emplace_back(gameState->grabbedCard->getPileParent(), gameState->grabbedCard);

        gameState->grabbedCard->select();
        if(gameState->grabbedCard->getPileParent())
            gameState->grabbedCard->removeFromPile();

        // Pop the card to the front
        gameState->grabbedCard->raiseToFront();

        glm::vec2 cardPosition = gameState->grabbedCard->getWorldTransform().getPosition2();
        gameState->grabStartPosition = position;
        gameState->grabOffset = glm::vec2(position.x - cardPosition.x,
                               position.y - cardPosition.y);

        gameState->updateSelectedUIGrid(gameState->grabbedCard);
        gameState->swapUIGrid();
    }
    else
    {
        gameState->grabbedCard = nullptr;
    }

    state = SA_PRESSED;
    std::cout << "GRABBING" << std::endl;
    gameState->cursor->setTarget(gameState->grabbedCard);
}

void SPSelectAction::drag(glm::vec2 position) {
    if(!gameState->grabbedCard)
        return;

    glm::vec2 difference = position - gameState->grabStartPosition;
    float differenceLen = glm::length(position - gameState->grabStartPosition);
    if (differenceLen <= DRAG_THRESHOLD)
        return;

    gameState->grabbedCard->getTransform()->setPosition2(
            glm::vec2(position.x - gameState->grabOffset.x, position.y - gameState->grabOffset.y));
    state = SA_DRAGGING;
    gameState->cursor->setTarget(gameState->grabbedCard);
    std::cout << "DRAGGING" << std::endl;
}

void SPSelectAction::drag()
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

void SPSelectAction::release()
{
    glm::vec2 position = InputManager::getCursorPosition();

    // If not dragging a card on release, this is a click
    if(!isDraggingCard())
    {
        SPCard* topmostCard = getTopmostCardAtPosition(position);

        // If the card clicked on is the top of the deck, send a deal event
        if(topmostCard && topmostCard->getPileRoot() == gameState->stock)
            EventManager::getInstance()->broadcastEvent(Event(SPEvent::EVT_DEAL));
        return;
    }

    gameState->clearGhostCards();
    SPPilable* bestPilable = gameState->selectAction->gameState->grabbedCard->getClosestOverlap();
    if(bestPilable)
    {
        state = SA_IDLE;

        SPPilable* parent = gameState->grabbedCard->getClosestOverlap();
        SPCard* child = gameState->grabbedCard;

        if(!parent)
        {
            EventManager::getInstance()->broadcastEvent(Event(SPEvent::EVT_CANCEL));
            return;
        }

//    SPPilable* oldParent = gameState.moveList.back().parent;
        parent->addToPile(child/*, inputMode == IM_GAMEPAD*/);

//    if(inputMode == IM_GAMEPAD)
//    {
//        child->deselect();
//        handleCompleteSuitIfFound(child);
//    }

        // Flip face down cards in play when their child is removed
        if(auto oldParentCard = dynamic_cast<SPCard*>(child->getPilePrevParent()))
        {
            // Check that the player didn't put the card back where it came from
            if(parent != oldParentCard && oldParentCard->isFaceDown())
            {
                oldParentCard->flip();
            }

            if(gameState->moveList.back().child && gameState->moveList.back().parent &&
               gameState->moveList.back().child == child &&
               gameState->moveList.back().parent == parent)
            {
                // Forget the previous move, player put the card back
                gameState->moveList.pop_back();
                return;
            }
        }

        gameState->unselectedUIGrid.clearBearing();
        gameState->updateUnselectedUIGrid();
    }
    //TODO: Broadcast an undo event to trigger the undo action
    else
        EventManager::getInstance()->broadcastEvent(Event(SPEvent::EVT_CANCEL));


    gameState->swapUIGrid();
    gameState->selectAction->gameState->grabbedCard = nullptr;
    gameState->selectAction->state = SA_IDLE;
    std::cout << "IDLE" << std::endl;
}

void SPSelectAction::eventCallback(Event event) {
    switch(event)
    {
        case SPEvent::EVT_PRESS:
            press();
            return;
        case SPEvent::EVT_DRAG:
            drag();
            return;
        case SPEvent::EVT_RELEASE:
            release();
            return;
        default:
            break;
    }

    Entity* newTarget = nullptr;

    switch(event)
    {
        case Event::EVT_SWITCH_INPUT_MODE:
            EventManager::getInstance()->broadcastEvent(Event(SPEvent::EVT_CANCEL));
            if(InputManager::getInstance()->getInputMode() == IM_GAMEPAD)
                gameState->cursor->setTarget(gameState->tableaus[0]->getPileEnd());
            return;
        case SPEvent::EVT_LEFT:
            newTarget = gameState->activeUIGrid->getElementToLeft(gameState->cursor->getTarget());
            break;
        case SPEvent::EVT_RIGHT:
            newTarget = gameState->activeUIGrid->getElementToRight(gameState->cursor->getTarget());
            break;
        case SPEvent::EVT_UP:
            newTarget = gameState->activeUIGrid->getElementAbove(gameState->cursor->getTarget());
            break;
        case SPEvent::EVT_DOWN:
            newTarget = gameState->activeUIGrid->getElementBelow(gameState->cursor->getTarget());
            break;
        default:
            return;
    }

    InputManager::setCursorPosition(gameState->cursor->getWorldTransform().getPosition2().x,
                                    gameState->cursor->getWorldTransform().getPosition2().y);

    if(!newTarget)
        return;

    gameState->cursor->setTarget(newTarget);

    // Move the grabbed card and point to it
    if (gameState->grabbedCard)
    {
        glm::vec2 cursorPosition = gameState->cursor->getWorldTransform().getPosition2();
        auto offset = glm::vec3(0);
        if(auto* pilableTarget = dynamic_cast<SPPilable*>(newTarget))
        {
            if (pilableTarget == pilableTarget->getPileRoot())
                offset = pilableTarget->getRootOffset();
            else
                offset = pilableTarget->getPileOffset();
        }

        glm::vec2 newPosition = cursorPosition + glm::vec2(offset.x, offset.y);

        drag(newPosition);
        gameState->cursor->setTarget(gameState->grabbedCard);
    }
}
