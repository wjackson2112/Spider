//
// Created by will on 3/19/26.
//

#ifndef NS_RESET_ACTION_H
#define NS_RESET_ACTION_H

#include "CFAction.h"
class NSGameMode;

class NSResetAction : public CFAction<NSGameMode>, IAnimationCompleteReceiver
{
    using CFAction::CFAction;

public:
    // IEventReceiver Interface
    void eventCallback(Event event) override;
};

#endif //NS_RESET_ACTION_H
