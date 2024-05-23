//
// Created by Will on 4/18/2024.
//

#ifndef SP_GAMEPLAY_ACTION_H
#define SP_GAMEPLAY_ACTION_H

#include "SPCard.h"
#include "SPSnapValidatorFourSuits.h"

enum SPActionState
{
    SA_IDLE,
    SA_PRESSED,
    SA_DRAGGING
};

struct SPEvent : Event {
public:
    enum
    {
        EVT_CANCEL = Event::EVT_EVENT_LAST,
        EVT_DEAL,
        EVT_UNDO,
        EVT_LEFT,
        EVT_RIGHT,
        EVT_UP,
        EVT_DOWN
    };
};

class SPGameplayAction : public IEventReceiver
{
protected:
    SPGameState* gameState;

public:
    SPActionState state = SA_IDLE;

    SPGameplayAction(SPGameState* gameState) : gameState(gameState){}

protected:
    SPCard* getTopmostCardAtPosition(glm::vec2 position);
};

#endif //SP_GAMEPLAY_ACTION_H
