//
// Created by Will on 5/5/2024.
//

#include "SPUndoAction.h"
#include "InputManager.h"


void SPUndoAction::undo() {
    if(gameState->moveList.empty())
        return;

    if(gameState->grabbedCard)
    {
        EventManager::getInstance()->broadcastEvent(Event(SPEvent::EVT_CANCEL));
        return;
    }

    MoveEntry currentEntry = gameState->moveList.back(); gameState->moveList.pop_back();

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
        for(SPPile* foundation : gameState->foundations)
            if(currentEntry.child->getPileParent() == foundation)
                isSuitMove = true;
        currentEntry.child->removeFromPile();
    }

    // TODO: Should raiseToFront just be a part of addToPile?
    currentEntry.child->raiseToFront();
    currentEntry.parent->addToPile(currentEntry.child, false, gameState->gameMode);

    if(auto* parentCard = dynamic_cast<SPCard*>(currentEntry.parent))
        if(currentEntry.parentFaceUp != parentCard->isFaceUp())
            parentCard->flip();

    // Ensure all moved cards are receiving updates
    auto* currCard = dynamic_cast<SPCard*>(currentEntry.child);
    while(currCard)
    {
        currCard = dynamic_cast<SPCard*>(currCard->getPileChild());
    }

    // Undo again if it's a suit move to break the suited stack
    if(isSuitMove)
        undo();
    else
        gameState->updateUnselectedUIGrid();

    if(InputManager::getInstance()->getInputMode() == IM_GAMEPAD)
        currentEntry.child->deselect();
}

void SPUndoAction::undoDeal(int numCards) {
    for(int i = 0; i < numCards; i++)
    // for(auto iter = gameState->tableaus.rbegin(); iter != gameState->tableaus.rend(); iter++)
    {
        SPPile* tableau = gameState->tableaus[i];
        SPCard* card = dynamic_cast<SPCard*>(tableau->getPileEnd());
        card->removeFromPile();
        card->raiseToFront();
        gameState->stock->addToPile(card, false, gameState->gameMode);

        if(gameState->cursor->getTarget() == card)
            gameState->cursor->setTarget(gameState->activeUIGrid->getElementBelow(card));
    }

    gameState->updateUnselectedUIGrid();
}

void SPUndoAction::eventCallback(Event event) {
    switch(event)
    {
        case SPEvent::EVT_UNDO:
            undo();
            break;
        default:
            break;
    }
}
