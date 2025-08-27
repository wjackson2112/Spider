//
// Created by will on 8/17/25.
//

#ifndef NS_GAME_H
#define NS_GAME_H

#include "Game.h"
#include "IEventReceiver.h"

class NSGame : public Game, IEventReceiver
{
    bool shouldStartNewGame = false;
    bool shouldReturnToMenu = false;

public:
    NSGame();

    void update() override;

    //IEventReceiver
    void eventCallback(Event event) override;
};

#endif //NS_GAME_H
