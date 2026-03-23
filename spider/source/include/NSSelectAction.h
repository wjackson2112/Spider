//  Copyright (c) 2026 William Jackson
//  Licensed under the MIT License

//
// Created by will on 8/20/25.
//

#ifndef NS_SELECT_ACTION_H
#define NS_SELECT_ACTION_H

#include "CFAction.h"
#include "CFCard.h"
// #include "NSGameState.h"

class NSGameMode;

class NSSelectAction : public CFAction<NSGameMode>
{
    bool isDraggingCard();
    bool validate(CFCard* grabbedCard);
public:
    explicit NSSelectAction(NSGameMode* gameMode) : CFAction(gameMode) {};
    ~NSSelectAction() override {}

    void drag(glm::vec2 position);
    void press();
    void drag();
    void release();

    bool validateRelease(CFPilable* parent, CFPilable* child);

    // IEventReceiver Interface
    void eventCallback(Event event) override;
};

#endif //NS_SELECT_ACTION_H
