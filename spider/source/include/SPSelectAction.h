//
// Created by Will on 4/7/2024.
//

#ifndef SP_SELECT_ACTION_H
#define SP_SELECT_ACTION_H

#include "SPGameplayAction.h"
#include "SPCard.h"

class SPSelectAction : public SPGameplayAction
{
private:
    bool isDraggingCard();
    bool validate(SPCard* grabbedCard);

public:
    explicit SPSelectAction(SPGameState* gameState) : SPGameplayAction(gameState) {};

    void drag(glm::vec2 position);
    void press();
    void drag();
    void release();

    // IEventReceiver Interface
    void eventCallback(Event event) override;
};

#endif //SP_SELECT_ACTION_H
