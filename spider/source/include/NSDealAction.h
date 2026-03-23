//  Copyright (c) 2026 William Jackson
//  Licensed under the MIT License

//
// Created by will on 8/22/25.
//

#ifndef NS_DEAL_ACTION_H
#define NS_DEAL_ACTION_H

#include "CFAction.h"
class NSGameMode;

class NSDealAction : public CFAction<NSGameMode>, IAnimationCompleteReceiver
{
    using CFAction::CFAction;
    // public:
    //     explicit SPDealAction(CFGameState* gameState) : SPGameplayAction(gameState) {};

private:
    void deal();
    void dealComplete(std::string identifier);

public:
    // IEventReceiver Interface
    void eventCallback(Event event) override;
};

#endif //NS_DEAL_ACTION_H
