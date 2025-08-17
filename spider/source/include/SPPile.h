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
     using CFPile::CFPile;
};

#endif //SP_PILE_H
