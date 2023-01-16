//
// Created by Will on 1/6/2023.
//

#include "SPScene.h"
#include "SPBackground.h"
#include "SPCard.h"
#include "SPPile.h"
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
    entityManager->registerEntity(this, new SPCard(glm::vec2(110.f, 10.f), SPSUIT_CLUB, SPVALUE_QUEEN, validator));
    entityManager->registerEntity(this, new SPCard(glm::vec2(210.f, 10.f), SPSUIT_DIAMOND, SPVALUE_JACK, validator));
    entityManager->registerEntity(this, new SPCard(glm::vec2(310.f, 10.f), SPSUIT_HEART, SPVALUE_KING, validator));
    entityManager->registerEntity(this, new SPCard(glm::vec2(410.f, 10.f), SPSUIT_HEART, SPVALUE_ACE, validator));
    entityManager->registerEntity(this, new SPCard(glm::vec2(510.f, 10.f), SPSUIT_HEART, SPVALUE_TWO, validator));

    SPCard* card = new SPCard(glm::vec2(0.f, 0.f), SPSUIT_SPADE, SPVALUE_KING, validator);
    SPPile* pile = new SPPile(glm::vec2(10.f, 100.f),
                              glm::vec3(0.0f, 0.0f, STACK_OFFSET),
                              glm::vec3(0.0f, 20.0f, STACK_OFFSET),
                              false, validator);

    pile->addToPile(card);

    entityManager->registerEntity(this, card);
    entityManager->registerEntity(this, pile);
    entityManager->registerEntity(this, new SPPile(glm::vec2(110.f, 100.f),
                                                   glm::vec3(0.0f, 0.0f, STACK_OFFSET),
                                                   glm::vec3(0.0f, 20.0f, STACK_OFFSET),
                                                   false, validator));
    entityManager->registerEntity(this, new SPPile(glm::vec2(10.f, 200.f),
                                                   glm::vec3(0.0f, 0.0f, STACK_OFFSET),
                                                   glm::vec3(0.0f, 0.0f, STACK_OFFSET),
                                                   true, validator));
}