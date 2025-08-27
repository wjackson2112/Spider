//
// Created by will on 8/25/25.
//

#include "NSDealAction.h"
#include "CFCard.h"
#include "CFDeck.h"
#include "NSGameMode.h"

#include "NSGameState.h"
#include "NSEvent.h"

void NSDealAction::deal()
{
    // No dealing if any of the tableaus are empty
    for(auto* tableau : gameMode->gameState->tableaus)
        if(!tableau->getPileChild())
            return;

    int numCardsDealt = 0;
    for(auto* tableau : gameMode->gameState->tableaus)
    {
        if (gameMode->gameState->stock->getDeckSize() > 0) {
            numCardsDealt++;
            CFCard* card = dynamic_cast<CFCard*>(gameMode->gameState->stock->getPileEnd());
            card->raiseToFront();
            card->flip();
            tableau->addToPile(card, false, gameMode->gameState->gameMode);
        }
    }
    gameMode->gameState->moveList.emplace_back(MoveEntry(numCardsDealt));

    // gameState->unselectedUIGrid.clearBearing();
    // gameState->updateUnselectedUIGrid();
}

void NSDealAction::eventCallback(Event event) {
    switch(event)
    {
        case NSEvent::EVT_DEAL:
            deal();
            break;
        default:
            break;
    }
}
