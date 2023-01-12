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
    entityManager->registerEntity(this, new SPCard(glm::vec2(10.f, 10.f), SPSUIT_HEART, SPVALUE_ACE));
    entityManager->registerEntity(this, new SPCard(glm::vec2(60.f, 10.f), SPSUIT_SPADE, SPVALUE_ACE));
}