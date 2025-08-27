//
// Created by will on 8/22/25.
//

#ifndef NS_DEAL_ACTION_H
#define NS_DEAL_ACTION_H

#include "CFAction.h"
class NSGameMode;

class NSDealAction : public CFAction<NSGameMode>
{
    using CFAction::CFAction;
    // public:
    //     explicit SPDealAction(CFGameState* gameState) : SPGameplayAction(gameState) {};

private:
    void deal();

public:
    // IEventReceiver Interface
    void eventCallback(Event event) override;
};

#endif //NS_DEAL_ACTION_H
