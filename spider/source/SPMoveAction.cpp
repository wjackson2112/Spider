//
// Created by Will on 5/10/2024.
//

#include "SPMoveAction.h"

void SPMoveAction::release(InputEvent event) {

    Entity* newTarget = nullptr;
    glm::vec2 cursorPosition;

    switch(event.event)
    {
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
            break;
    }

    if(!newTarget)
        return;

    gameState->cursor->setTarget(newTarget);
    cursorPosition = gameState->cursor->getWorldTransform().getPosition2();

    auto offset = glm::vec3(0);
    if(auto* pilableTarget = dynamic_cast<SPPilable*>(newTarget))
    {
        if (pilableTarget == pilableTarget->getPileRoot())
            offset = pilableTarget->getRootOffset();
        else
            offset = pilableTarget->getPileOffset();
    }

    event.position = cursorPosition + glm::vec2(offset.x, offset.y);

    if (gameState->grabbedCard)
    {
        // TODO: Fix
//        gameState->selectAction->drag(event);
//                                gameState.grabbedCard->getTransform()->setPosition2(glm::vec2(cursorPosition.x + offset.x - gameState.select->grabOffset.x,
//                                                                                    cursorPosition.y + offset.y - gameState.select->grabOffset.y));
        gameState->cursor->setTarget(gameState->grabbedCard);
    }
}
