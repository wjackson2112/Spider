//
// Created by Will on 1/15/2023.
//

#include "SPSnapComponent.h"

void SPSnapComponent::snap(SPCard* card)
{
    card->setParent(this->parent);
    card->getTransform()->setPosition(glm::vec3(transform.getPosition().x,
                                                transform.getPosition().y,
                                                STACK_OFFSET));
    snappedCard = card;
}

void SPSnapComponent::unsnap()
{
    snappedCard->setParent(nullptr);
    snappedCard = nullptr;
}

bool SPSnapComponent::isInSnapChain(SPCard *card)
{
    SPCard *currCard = snappedCard;
    while(currCard != nullptr)
    {
        if (currCard == card)
            return true;
        currCard = currCard->getComponent<SPSnapComponent>()->snappedCard;
    }
    return false;
}