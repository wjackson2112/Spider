//  Copyright (c) 2026 William Jackson
//  Licensed under the MIT License

//
// Created by Will on 4/18/2024.
//

#include "SPGameplayAction.h"

// SPCard* SPGameplayAction::getTopmostCardAtPosition(glm::vec2 position)
// {
//     std::vector<Entity*> entities = gameState->owningScene->getEntities();
//     for(Entity* entity : entities)
//     {
//         auto* card = dynamic_cast<SPCard*>(entity);
//         if(!card)
//             continue;
//
//         if(card->isTopmostAtPoint(position))
//             return card;
//     }
//
//     return nullptr;
// }