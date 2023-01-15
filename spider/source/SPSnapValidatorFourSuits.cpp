//
// Created by Will on 1/14/2023.
//

#include "SPSnapValidatorFourSuits.h"

#include "SPCard.h"

bool SPSnapValidatorFourSuits::validate(Entity* parent, Entity* child)
{
    if(SPCard* childCard = dynamic_cast<SPCard*>(child))
    {
        if(SPCard* parentCard = dynamic_cast<SPCard*>(parent))
        {
            bool suitsAlternate = (parentCard->getSuit() % 2) != (childCard->getSuit() % 2);
            bool valuesDecrease = parentCard->getValue() == childCard->getValue() + 1;
            if(suitsAlternate && valuesDecrease)
                return true;
            return false;
        }
    }

    return false;
}