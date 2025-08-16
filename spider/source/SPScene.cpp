//
// Created by Will on 1/6/2023.
//

#include "SPScene.h"
#include "SPBackground.h"
#include "SPCard.h"
#include "SPPile.h"
#include "SPSnapValidatorFourSuits.h"
#include "TextComponent.h"
#include "AssetManager.h"

//#include "EntityManager.h"
#include "CameraEntity2D.h"
#include "CFBackground.h"
#include "SPFrameCounterEntity.h"

SPScene::SPScene()
{
//    EntityManager* entityManager = EntityManager::getInstance();
    addEntity<CameraEntity2D>();
    addEntity<CFBackground>();
    addEntity<SPFrameCounterEntity>();
    auto validator = addEntity<SPSnapValidatorFourSuits>();
    validator->initialSetup(this);
//    entityManager->registerEntity(this, new CameraEntity2D());
//    entityManager->registerEntity(this, new SPBackground());
//    entityManager->registerEntity(this, new SPFrameCounterEntity());
//
//    auto validator = new SPSnapValidatorFourSuits();
//
//    validator->initialSetup(this);
//
//    // TODO: This currently has to go last to avoid weird depth on the text, fix rendering so this doesn't happen
//    entityManager->registerEntity(this, validator);
}