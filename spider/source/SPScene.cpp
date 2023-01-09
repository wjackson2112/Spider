//
// Created by Will on 1/6/2023.
//

#include "SPScene.h"
#include "SPBackground.h"
#include "SPCard.h"

#include "EntityManager.h"
#include "CameraEntity2D.h"

SPScene::SPScene()
{
    EntityManager* entityManager = EntityManager::getInstance();
    entityManager->registerEntity(this, new CameraEntity2D());
    entityManager->registerEntity(this, new SPBackground());
    entityManager->registerEntity(this, new SPCard());
}