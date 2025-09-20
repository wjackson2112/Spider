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
    // Can't start a deal while another is in progress
    if(gameMode->gameState->stock->isDealing())
        return;

    // No dealing if any of the tableaus are empty
    // for(auto* tableau : gameMode->gameState->tableaus)
    //     if(!tableau->getPileChild())
    //         return;

    int numCardsDealt = 0;
    for(auto* tableau : gameMode->gameState->tableaus)
    {
        if (gameMode->gameState->stock->getDeckSize() > 0) {
            numCardsDealt++;
            CFCard* card = dynamic_cast<CFCard*>(gameMode->gameState->stock->getPileEnd());
            card->raiseToFront();
            card->flip();
            tableau->addToPile(card, false, this, static_cast<AnimCompleteFunction>(&NSDealAction::dealComplete));
        }
    }
    gameMode->gameState->moveList.emplace_back(MoveEntry(numCardsDealt));

    // gameState->unselectedUIGrid.clearBearing();
    // gameState->updateUnselectedUIGrid();
}

void NSDealAction::dealComplete(std::string identifier, Entity* entity)
{
    // dynamic_cast<CFCard*>(entity)->flip();
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
