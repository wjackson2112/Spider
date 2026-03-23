//  Copyright (c) 2026 William Jackson
//  Licensed under the MIT License

//
// Created by will on 8/20/25.
//

#include "NSGameState.h"
#include "NSGameMode.h"

#include "SpriteComponent2D.h"
#include "AnimationComponent.h"
#include "ColorAnimation.h"

NSGameState::~NSGameState() = default;





// void NSGameState::updateUnselectedUIGrid()
// {
//     unselectedUIGrid.deregisterAllElements();
//
//     for(CFPile* tableau : tableaus)
//     {
//         if(tableau->getPileChild() == nullptr)
//         {
//             unselectedUIGrid.registerElement(tableau);
//             continue;
//         }
//
//         for(CFPilable* pilable = tableau->getPileEnd(); pilable != nullptr; pilable = pilable->getPileParent())
//         {
//             if(CFCard* card = dynamic_cast<CFCard*>(pilable))
//             {
//                 if(card->isFaceUp())
//                 {
//                     unselectedUIGrid.registerElement(card);
//                 }
//             }
//         }
//     }
// }
//
// void NSGameState::updateSelectedUIGrid(CFCard* selectedCard)
// {
//     selectedUIGrid.deregisterAllElements();
//
//     selectedUIGrid.registerElement(selectedCard->getPilePrevParent());
//
//     // for(SPPile* tableau : tableaus)
//     // {
//     //     if(validateRelease(tableau->getPileEnd(), selectedCard))
//     //         selectedUIGrid.registerElement(tableau->getPileEnd());
//     // }
// }
//
// void NSGameState::swapUIGrid()
// {
//     if(activeUIGrid == &unselectedUIGrid)
//         activeUIGrid = &selectedUIGrid;
//     else
//         activeUIGrid = &unselectedUIGrid;
// }

// void NSGameState::updateGhostCards(CFCard* selectedCard)
// {
//     for(CFPile* tableau : tableaus)
//     {
//         if(tableau->getPileEnd() == selectedCard)
//             continue;
//
//         if(validateRelease(tableau->getPileEnd(), selectedCard))
//         {
//             for(CFPilable* currPilable = selectedCard; currPilable != nullptr; currPilable = currPilable->getPileChild())
//             {
//                 auto* currCard = dynamic_cast<CFCard*>(currPilable);
//
//                 auto* ghostCard = owningScene->addEntity<CFCard>(glm::vec2(0,0), currCard->getSuit(), currCard->getRank(), true, this);
//                 auto* ghostSpriteComponent = ghostCard->getComponent<SpriteComponent2D>();
//                 auto* ghostAnimationComponent = ghostCard->getComponent<AnimationComponent>();
//
//                 tableau->addToPile(ghostCard, true);
//                 ghostSpriteComponent->setColor4(glm::vec4(0.5f, 0.75f, 0.5f, 0.0f));
//                 ghostAnimationComponent->addAndStart<ColorAnimation>(ghostCard, glm::vec4(0.5f, 0.75f, 0.5f, 0.25f), 0.1f);
//                 ghostCards.push_back(ghostCard);
//             }
//         }
//     }
//
//     gameMode->updateLayout();
// }
//
// void NSGameState::clearGhostCards()
// {
//     if(ghostCards.empty())
//         return;
//
//     for(auto it = ghostCards.begin(); it != ghostCards.end(); it++)
//     {
//         (*it)->removeFromPile();
//         glm::vec3 ghostPosition = (*it)->getTransform()->getPosition();
//         (*it)->getTransform()->setPosition(glm::vec3(ghostPosition.x, ghostPosition.y, ghostPosition.z - STACK_OFFSET/2));
//         (*it)->getComponent<AnimationComponent>()->skipAll();
//
// //        auto* ghostSpriteComponent = (*it)->getComponent<SpriteComponent2D>();
//         auto* ghostAnimationComponent = (*it)->getComponent<AnimationComponent>();
//         ghostAnimationComponent->addAndStart<ColorAnimation>((*it), glm::vec4(0.5f, 0.75f, 0.5f, 0.0f), 0.1f, this, (AnimCompleteFunction) &SPGameState::ghostCardClearComplete);
//     }
// }
//
// void NSGameState::ghostCardClearComplete(Entity* card)
// {
// //    owningScene->removeEntity(card);
//     card->shouldBeDestroyed = true;
//     ghostCards.erase(std::remove(ghostCards.begin(), ghostCards.end(), card), ghostCards.end());
// }

