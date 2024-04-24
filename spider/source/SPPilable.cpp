//
// Created by Will on 1/16/2023.
//

#include "SPPilable.h"

SPPilable::SPPilable()
{
    pileRoot = this;
    pileParent = nullptr;
    pilePrevParent = nullptr;
    pileChild = nullptr;
    pileEnd = this;
}

glm::vec3 SPPilable::getRootOffset()
{
    return getPileRoot()->rootOffset;
}

glm::vec3 SPPilable::getPileOffset()
{
    return getPileRoot()->pileOffset;
}

glm::vec3 SPPilable::getInitialRootOffset()
{
    return getPileRoot()->initialRootOffset;
}

glm::vec3 SPPilable::getInitialPileOffset()
{
    return getPileRoot()->initialPileOffset;
}

SPPilable* SPPilable::getPileRoot()
{
//    if(!pileRoot)
//    {
//        if (!pileParent)
//            pileRoot = this;
//        else
//            pileRoot = pileParent->getPileRoot();
//    }

    return pileRoot;
}

SPPilable* SPPilable::getPileEnd()
{
//    if(!pileEnd) {
//        if (!pileChild)
//            pileEnd = this;
//        else
//            pileEnd = pileChild->getPileEnd();
//    }

    return pileEnd;
}

void SPPilable::updatePileRoot(SPPilable* pilable)
{
    if(this != pileEnd)
        pileEnd->updatePileRoot(pilable);
    else
        this->updatePileRootRecurse(pilable);
}

void SPPilable::updatePileRootRecurse(SPPilable *pilable)
{
    pileRoot = pilable;
    if(pileParent)
        pileParent->updatePileRootRecurse(pilable);
}

void SPPilable::updatePileEnd(SPPilable* pilable)
{
    if(this != pileRoot)
        pileRoot->updatePileEnd(pilable);
    else
        this->updatePileEndRecurse(pilable);
}

void SPPilable::updatePileEndRecurse(SPPilable *pilable)
{
    pileEnd = pilable;
    if(pileChild)
        pileChild->updatePileEndRecurse(pilable);
}

void SPPilable::raiseToFront()
{
    glm::vec3 origPosition = transform.getPosition();
    transform.setPosition(glm::vec3(origPosition.x, origPosition.y, STACK_MAX));
}

void SPPilable::addToPile(SPPilable *pilable, bool snap)
{
    pilable->removeFromPile();

    if(this != getPileEnd())
    {
        pileEnd->addToPile(pilable, snap);
        return;
    }

    // if(this == pileEnd)
    glm::vec3 currWorldPosition = pilable->getWorldTransform().getPosition();

    pileChild = pilable;
    updatePileEnd(pileChild->getPileEnd());

    if(pilable->pileParent)
        pilable->pilePrevParent = pilable->pileParent;

    pilable->pileParent = this;
    updatePileRoot(pileRoot);

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
    pileParent->pileChild = nullptr;
    pileParent->pileEnd = pileParent;
    pileParent->updatePileEnd(pileParent);
    if(pileParent)
        pilePrevParent = pileParent;
    pileParent = nullptr;
    pileRoot = this;

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

void SPPilable::select()
{
    selected = true;
}

void SPPilable::deselect()
{
    selected = false;
}