//
// Created by Will on 1/16/2023.
//

#include "SPPilable.h"

glm::vec3 SPPilable::getRootOffset()
{
    return getPileRoot()->rootOffset;
}

glm::vec3 SPPilable::getPileOffset()
{
    return getPileRoot()->pileOffset;
}

SPPilable* SPPilable::getPileRoot()
{
    if(!pileParent)
        return this;

    return pileParent->getPileRoot();
}

SPPilable *SPPilable::getPileEnd() {
    if(!pileChild)
        return this;
    return pileChild->getPileEnd();
}

void SPPilable::addToPile(SPPilable *pilable)
{
    SPPilable* pileEnd = getPileEnd();

    pileEnd->pileChild = pilable;
    pilable->pileParent = pileEnd;
    pileEnd->addChild(pilable);

    if(this == getPileRoot())
        pilable->getTransform()->setPosition(getRootOffset());
    else
        pilable->getTransform()->setPosition(getPileOffset());
}

bool SPPilable::isInPile(SPPilable *pilable)
{
    SPPilable* pileRoot = getPileRoot();

    SPPilable* currPilable = pileRoot;
    while(currPilable != pilable && currPilable != nullptr)
        currPilable = currPilable->pileChild;

    return currPilable == pilable;
}

void SPPilable::removeFromPile()
{
    if(pileParent == nullptr)
        return;

    Transform worldTransform = this->getWorldTransform();

    pileParent->removeChild(this);
    pileParent->pileChild= nullptr;
    pileParent = nullptr;

    transform = worldTransform;
}