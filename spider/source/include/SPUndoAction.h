//
// Created by Will on 5/5/2024.
//

#ifndef SP_UNDO_ACTION_H
#define SP_UNDO_ACTION_H

#include "SPGameplayAction.h"
#include "SPCard.h"

class SPUndoAction : public SPGameplayAction
{
    using SPGameplayAction::SPGameplayAction;
// public:
//     explicit SPUndoAction(CFGameState* gameState) : SPGameplayAction(gameState) {};

private:
    void undo();
    void undoDeal(int numCards);

public:
    // IEventReceiver Interface
    void eventCallback(Event event) override;
};

#endif //SP_UNDO_ACTION_H
