//  Copyright (c) 2026 William Jackson
//  Licensed under the MIT License

//
// Created by will on 8/22/25.
//

#ifndef NS_CANCEL_ACTION_H
#define NS_CANCEL_ACTION_H

#include "CFAction.h"

class NSGameMode;

class NSCancelAction : public CFAction<NSGameMode>
{
    using CFAction::CFAction;
    // public:
    // explicit SPCancelAction(CFGameState* gameState) : SPGameplayAction(gameState) {};

private:
    void cancel();

public:
    // IEventReceiver Interface
    void eventCallback(Event event) override;
};

#endif //NS_CANCEL_ACTION_H
