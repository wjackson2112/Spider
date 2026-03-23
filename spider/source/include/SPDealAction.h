//  Copyright (c) 2026 William Jackson
//  Licensed under the MIT License

//
// Created by Will on 5/3/2024.
//

#ifndef SP_DEAL_ACTION_H
#define SP_DEAL_ACTION_H

#include "SPGameplayAction.h"

class SPDealAction : public SPGameplayAction
{
    using SPGameplayAction::SPGameplayAction;
// public:
//     explicit SPDealAction(CFGameState* gameState) : SPGameplayAction(gameState) {};

private:
    void deal();

public:
    // IEventReceiver Interface
    void eventCallback(Event event) override;
};

#endif //SP_DEALACTION_H
