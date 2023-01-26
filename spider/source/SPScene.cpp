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

#include "EntityManager.h"
#include "CameraEntity2D.h"
#include "SPFrameCounterEntity.h"

SPScene::SPScene()
{
    EntityManager* entityManager = EntityManager::getInstance();
    SPSnapValidatorFourSuits* validator = new SPSnapValidatorFourSuits();
    entityManager->registerEntity(this, new CameraEntity2D());
    entityManager->registerEntity(this, new SPBackground());

    entityManager->registerEntity(this, new SPFrameCounterEntity());

    validator->initialSetup(this);
}