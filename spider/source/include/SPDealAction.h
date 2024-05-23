//
// Created by Will on 5/3/2024.
//

#ifndef SP_DEAL_ACTION_H
#define SP_DEAL_ACTION_H

#include "SPGameplayAction.h"
#include "SPCard.h"

class SPDealAction : public SPGameplayAction
{
public:
    explicit SPDealAction(SPGameState* gameState) : SPGameplayAction(gameState) {};

private:
    void deal();

public:
    // IEventReceiver Interface
    void eventCallback(Event event) override;
};

#endif //SP_DEALACTION_H
