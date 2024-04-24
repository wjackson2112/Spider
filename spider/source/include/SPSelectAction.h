//
// Created by Will on 4/7/2024.
//

#ifndef SP_GRAB_ACTION_H
#define SP_GRAB_ACTION_H

#include "SPGameplayAction.h"
#include "SPSnapValidatorFourSuits.h"

class SPSelectAction : SPGameplayAction
{
private:
    bool validate(SPCard* grabbedCard);

public:
    SPSelectAction(SPGameState* gameState) : SPGameplayAction(gameState) {};

    // PlayerAction Interface
    void execute(InputEvent event) override;
};

#endif //SP_GRAB_ACTION_H
