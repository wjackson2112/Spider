//  Copyright (c) 2026 William Jackson
//  Licensed under the MIT License

//
// Created by will on 8/25/25.
//

#include "NSCancelAction.h"
#include "EventManager.h"

#include "NSGameMode.h"
#include "NSEvent.h"

void NSCancelAction::cancel() {
    if (gameMode->gameState->grabbedCard)
    {
        gameMode->gameState->grabbedCard = nullptr;
        // gameState->clearGhostCards();
        EventManager::getInstance()->broadcastEvent(Event(NSEvent::EVT_UNDO));
        // gameState->swapUIGrid();
    }
}

void NSCancelAction::eventCallback(Event event) {
    switch(event)
    {
        case NSEvent::EVT_CANCEL:
            cancel();
            break;
        default:
            break;
    }
}