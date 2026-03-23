//  Copyright (c) 2026 William Jackson
//  Licensed under the MIT License

//
// Created by will on 8/22/25.
//

#ifndef NS_UNDO_ACTION_H
#define NS_UNDO_ACTION_H

#include "CFAction.h"

class NSGameMode;

class NSUndoAction : public CFAction<NSGameMode>
{
    using CFAction::CFAction;
    // public:
    //     explicit SPUndoAction(CFGameState* gameState) : SPGameplayAction(gameState) {};

private:
    void undo();
    void undoDeal(int numCards);

public:
    // IEventReceiver Interface
    void eventCallback(Event event) override;
};

#endif //NS_UNDO_ACTION_H
