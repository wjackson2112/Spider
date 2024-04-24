//
// Created by Will on 4/7/2024.
//

#include "SPSelectAction.h"
#include <iostream>

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

void SPSelectAction::execute(InputEvent event)
{
    SPCard* grabbedCard = getTopmostCardAtPosition(event.position);

    if (grabbedCard && validate(grabbedCard))
    {
        gameState->moveList.emplace_back(grabbedCard->getPileParent(), grabbedCard);

        grabbedCard->select();
        if(grabbedCard->getPileParent())
            grabbedCard->removeFromPile();

        // Pop the card to the front
        grabbedCard->raiseToFront();

        glm::vec2 cardPosition = grabbedCard->getWorldTransform().getPosition2();
        gameState->grabStartPosition = event.position;
        gameState->grabOffset = glm::vec2(event.position.x - cardPosition.x,
                                          event.position.y - cardPosition.y);
    }
    else
    {
        grabbedCard = nullptr;
    }

    gameState->grabbedCard = grabbedCard;
}