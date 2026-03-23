//  Copyright (c) 2026 William Jackson
//  Licensed under the MIT License

//
// Created by Will on 5/3/2024.
//

#include "SPDealAction.h"
#include "CFCard.h"

void SPDealAction::deal()
{
    // No dealing if any of the tableaus are empty
    for(auto* tableau : gameState->tableaus)
        if(!tableau->getPileChild())
            return;

    int numCardsDealt = 0;
    for(auto* tableau : gameState->tableaus)
    {
        if (gameState->stock->getDeckSize() > 0) {
            numCardsDealt++;
            CFCard* card = dynamic_cast<CFCard*>(gameState->stock->getPileEnd());
            card->raiseToFront();
            card->flip();
            tableau->addToPile(card, false, gameState->gameMode);
        }
    }
    gameState->moveList.emplace_back(MoveEntry(numCardsDealt));

    gameState->unselectedUIGrid.clearBearing();
    gameState->updateUnselectedUIGrid();
}

void SPDealAction::eventCallback(Event event) {
    switch(event)
    {
        case SPEvent::EVT_DEAL:
            deal();
            break;
        default:
            break;
    }
}
