//
// Created by Will on 1/16/2023.
//

#ifndef SP_PILE_H
#define SP_PILE_H

#include "SPCard.h"
#include "SPPilable.h"
#include "CFPile.h"
#include "SPSnapValidator.h"
#include "Entity.h"
#include "ICollisionReceiver.h"

class SPPile : public CFPile//, public ICollisionReceiver
{
     // using CFPile::CFPile;

protected:
//     glm::vec2 size;
     SPSnapValidator* validator;
//
public:
     bool incrementing = false;
//
//     //SPPilable
//     glm::vec2 getSize() { return size; }
//     void setSize(glm::vec2 size);
//
     SPPile(glm::vec2 position, glm::vec3 rootOffset, glm::vec3 pileOffset, bool incrementing, SPSnapValidator* validator);
//     ~SPPile();
};

#endif //SP_PILE_H
