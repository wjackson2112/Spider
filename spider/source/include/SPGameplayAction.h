//
// Created by Will on 4/18/2024.
//

#ifndef SP_GAMEPLAY_ACTION_H
#define SP_GAMEPLAY_ACTION_H

#include "PlayerAction.h"
#include "SPCard.h"
#include "SPSnapValidatorFourSuits.h"

class SPGameplayAction : PlayerAction
{
protected:
    SPGameState* gameState;

public:
    SPGameplayAction(SPGameState* gameState) : gameState(gameState){}

    virtual void execute(InputEvent event) = 0;

protected:
    SPCard* getTopmostCardAtPosition(glm::vec2 position);
};

#endif //SP_GAMEPLAY_ACTION_H
