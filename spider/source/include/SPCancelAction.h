//
// Created by Will on 5/5/2024.
//

#ifndef SP_CANCEL_ACTION_H
#define SP_CANCEL_ACTION_H

#include "SPGameplayAction.h"
#include "SPCard.h"
#include "InputEvent.h"

class SPCancelAction : public SPGameplayAction
{
    using SPGameplayAction::SPGameplayAction;
    // public:
    // explicit SPCancelAction(CFGameState* gameState) : SPGameplayAction(gameState) {};

private:
    void cancel();
    
public:
    // IEventReceiver Interface
    void eventCallback(Event event) override;
};

#endif //SP_CANCEL_ACTION_H
