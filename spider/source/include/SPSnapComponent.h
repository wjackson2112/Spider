//
// Created by Will on 1/15/2023.
//

#ifndef SP_SNAP_COMPONENT_H
#define SP_SNAP_COMPONENT_H

#include "CollisionComponent2DAABB.h"
#include "ICollisionReceiver.h"
#include "SPCard.h"

class SPSnapComponent : public CollisionComponent2DAABB, public ICollisionReceiver
{
    SPCard* snappedCard = nullptr;

public:
    SPSnapComponent(glm::vec2 size)
    : CollisionComponent2DAABB(this, size)
    {}

    SPCard* getSnappedCard() { return snappedCard; }
    void snap(SPCard* other);
    void unsnap();
    bool isInSnapChain(SPCard *card);

    //ICollisionReceiver
    virtual void collisionCallback(ICollisionReceiver* collisionReceiver, glm::vec3 overlap) {}
};

#endif //SP_SNAP_COMPONENT_H
