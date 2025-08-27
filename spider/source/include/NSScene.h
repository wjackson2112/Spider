//
// Created by will on 8/17/25.
//

#ifndef NS_SCENE_H
#define NS_SCENE_H

#include "Scene.h"

#include "NSGameMode.h"

class NSScene : public Scene
{
    NSGameMode* gameMode;
    NSGameState* gameState;

public:
    NSScene();
};

#endif //NS_SCENE_H
