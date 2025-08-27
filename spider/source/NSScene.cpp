//
// Created by will on 8/17/25.
//

#include "NSScene.h"
#include "NSGameMode.h"

#include "CameraEntity2D.h"
#include "CFBackground.h"
#include "NSGame.h"
#include "SPFrameCounterEntity.h"

NSScene::NSScene()
{
    addEntity<CameraEntity2D>();
    addEntity<CFBackground>();
    addEntity<SPFrameCounterEntity>();

    gameState = new NSGameState();
    gameMode = new NSGameMode(gameState);

    // auto gameMode = addEntity<NSGameMode>();
    gameMode->initialSetup(this);
}