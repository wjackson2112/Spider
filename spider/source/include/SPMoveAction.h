//  Copyright (c) 2026 William Jackson
//  Licensed under the MIT License

//
// Created by Will on 5/10/2024.
//

#ifndef SP_MOVE_ACTION_H
#define SP_MOVE_ACTION_H

#include "SPGameplayAction.h"
#include "SPCard.h"

class SPMoveAction : public SPGameplayAction
{
public:
    explicit SPMoveAction(SPGameState* gameState) : SPGameplayAction(gameState) {}

    void release(InputEvent event);
};

#endif //SP_MOVE_ACTION_H
