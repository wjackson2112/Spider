//
// Created by will on 3/19/26.
//

#include "NSResetAction.h"

#include "NSEvent.h"
#include "NSGameMode.h"

void NSResetAction::eventCallback(Event event) {
    switch(event)
    {
        case NSEvent::EVT_WON_GAME:
        case NSEvent::EVT_RESET_GAME:
            gameMode->resetGame();
            break;
        default:
            break;
    }
}
