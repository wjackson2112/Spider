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

void SPPilable::raiseToFront()
{
    glm::vec3 origPosition = transform.getPosition();
    transform.setPosition(glm::vec3(origPosition.x, origPosition.y, STACK_MAX));
}

void SPPilable::addToPile(SPPilable *pilable, bool snap)
{
    SPPilable* pileEnd = getPileEnd();
    SPPilable* pileRoot = getPileRoot();

    pilable->removeFromPile();

    if(this == pileEnd)
    {
        glm::vec3 currWorldPosition = pilable->getWorldTransform().getPosition();
        pileChild = pilable;
        pilable->pileParent = this;
        addChild(pilable);

        // Attach to the pile without moving the card
        glm::vec3 attachedWorldPosition = pilable->getWorldTransform().getPosition();
        glm::vec3 worldPositionDifference = attachedWorldPosition - currWorldPosition;
        pilable->getTransform()->translate(-worldPositionDifference);

        // Move the card to the target location
        if(pilable->getPileParent() == pileRoot)
        {
            if(snap)
                pilable->snapTo(getRootOffset());
            else
                pilable->moveTo(getRootOffset());
        }
        else
        {
            if(snap)
                pilable->snapTo(getPileOffset());
            else
                pilable->moveTo(getPileOffset());
        }

        SPPilable* currPilable = pilable->getPileChild();
        while(currPilable != nullptr)
        {

            // Move the card to the target location
            if(currPilable->getPileParent() == pileRoot)
                currPilable->moveTo(getRootOffset());
            else
                currPilable->moveTo(getPileOffset());

            currPilable = currPilable->getPileChild();
        }
    }
    else
    {
        pileChild->addToPile(pilable, snap);
    }
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

void SPPilable::moveTo(glm::vec3 target)
{
    transform.setPosition(target);
}

void SPPilable::snapTo(glm::vec3 target)
{
    transform.setPosition(target);
}