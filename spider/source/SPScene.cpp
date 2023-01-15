//
// Created by Will on 1/6/2023.
//

#include "SPScene.h"
#include "SPBackground.h"
#include "SPCard.h"
#include "SPSnapValidatorFourSuits.h"

#include "EntityManager.h"
#include "CameraEntity2D.h"

SPScene::SPScene()
{
    EntityManager* entityManager = EntityManager::getInstance();
    SPSnapValidatorFourSuits* validator = new SPSnapValidatorFourSuits();
    entityManager->registerEntity(this, new CameraEntity2D());
    entityManager->registerEntity(this, new SPBackground());
    entityManager->registerEntity(this, new SPCard(glm::vec2(10.f, 10.f), SPSUIT_SPADE, SPVALUE_TEN, validator));
    entityManager->registerEntity(this, new SPCard(glm::vec2(110.f, 10.f), SPSUIT_HEART, SPVALUE_JACK, validator));
    entityManager->registerEntity(this, new SPCard(glm::vec2(210.f, 10.f), SPSUIT_DIAMOND, SPVALUE_JACK, validator));
    entityManager->registerEntity(this, new SPCard(glm::vec2(310.f, 10.f), SPSUIT_CLUB, SPVALUE_KING, validator));
}