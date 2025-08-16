//
// Created by Will on 5/5/2024.
//

#include "SPCancelAction.h"
#include "EventManager.h"

void SPCancelAction::cancel() {
    if (gameState->grabbedCard)
    {
        gameState->grabbedCard = nullptr;
        // gameState->clearGhostCards();
        EventManager::getInstance()->broadcastEvent(Event(SPEvent::EVT_UNDO));
        gameState->swapUIGrid();
    }
}

void SPCancelAction::eventCallback(Event event) {
    switch(event)
    {
        case SPEvent::EVT_CANCEL:
            cancel();
            break;
        default:
            break;
    }
}
