//
// Created by Will on 12/1/2022.
//

#ifndef SP_GAME_H
#define SP_GAME_H

#include "Game.h"
#include "IInputReceiver.h"
#include "IEventReceiver.h"

class SPGame : public Game, IEventReceiver
{
    bool shouldStartNewGame = false;

public:
    SPGame();

    void update() override;

    //IEventReceiver
    void eventCallback(Event event) override;
};

#endif //SP_GAME_H
