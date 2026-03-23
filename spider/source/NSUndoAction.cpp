//  Copyright (c) 2026 William Jackson
//  Licensed under the MIT License

//
// Created by will on 8/25/25.
//

#include "NSUndoAction.h"
#include "InputManager.h"

#include "CFPile.h"
#include "CFCard.h"
#include "CFDeck.h"

#include "NSGameMode.h"
#include "NSEvent.h"

void NSUndoAction::undo() {
    if(gameMode->gameState->moveList.empty())
        return;

    if(gameMode->gameState->grabbedCard)
    {
        EventManager::getInstance()->broadcastEvent(Event(NSEvent::EVT_CANCEL));
        return;
    }

    MoveEntry currentEntry = gameMode->gameState->moveList.back(); gameMode->gameState->moveList.pop_back();

    // If both are nullptr, this move was a deal
    if(currentEntry.parent == nullptr && currentEntry.child == nullptr)
    {
        undoDeal(currentEntry.numCards);
        return;
    }

    bool isSuitMove = false;
    if(currentEntry.child->getPileParent())
    {
        // Undo'ing a suited stack to the foundation
        for(CFPile* foundation : gameMode->gameState->foundations)
            if(currentEntry.child->getPileParent() == foundation)
                isSuitMove = true;
        currentEntry.child->removeFromPile();
    }

    // TODO: Should raiseToFront just be a part of addToPile?
    currentEntry.child->raiseToFront();
    currentEntry.parent->addToPile(currentEntry.child, false);//, gameMode->gameState->gameMode);

    if(auto* parentCard = dynamic_cast<CFCard*>(currentEntry.parent))
        if(currentEntry.parentFaceUp != parentCard->isFaceUp())
            parentCard->flip();

    // Ensure all moved cards are receiving updates
    auto* currCard = dynamic_cast<CFCard*>(currentEntry.child);
    while(currCard)
    {
        currCard = dynamic_cast<CFCard*>(currCard->getPileChild());
    }

    // Undo again if it's a suit move to break the suited stack
    if(isSuitMove)
        undo();
    // else
        // gameState->updateUnselectedUIGrid();

    if(InputManager::getInstance()->getInputMode() == IM_GAMEPAD)
        currentEntry.child->deselect();
}

void NSUndoAction::undoDeal(int numCards) {
    for(int i = 0; i < numCards; i++)
    // for(auto iter = gameState->tableaus.rbegin(); iter != gameState->tableaus.rend(); iter++)
    {
        CFPile* tableau = gameMode->gameState->tableaus[numCards-i-1];
        CFCard* card = dynamic_cast<CFCard*>(tableau->getPileEnd());
        card->removeFromPile();
        card->raiseToFront();
        if(card->isFaceUp())
            card->flip();
        gameMode->gameState->stock->addToPile(card, false);//, gameMode);

        // if(gameState->cursor->getTarget() == card)
        //     gameState->cursor->setTarget(gameState->activeUIGrid->getElementBelow(card));
    }

    // gameState->updateUnselectedUIGrid();
}

void NSUndoAction::eventCallback(Event event) {
    switch(event)
    {
        case NSEvent::EVT_UNDO:
            undo();
            break;
        default:
            break;
    }
}
