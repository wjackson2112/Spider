//
// Created by Will on 1/14/2023.
//

#include "SPSnapValidatorFourSuits.h"

#include "SPCard.h"
#include "SPPile.h"

bool SPSnapValidatorFourSuits::validate(SPPilable* parent, SPPilable* child)
{
    if(SPCard* childCard = dynamic_cast<SPCard*>(child))
    {
        if(SPPile* parentPile = dynamic_cast<SPPile*>(parent))
        {
            if(parentPile->incrementing && childCard->getValue() == SPVALUE_ACE)
                return true;
            else if(!parentPile->incrementing && childCard->getValue() == SPVALUE_KING)
                return true;
            return false;
        }

        if(SPCard* parentCard = dynamic_cast<SPCard*>(parent))
        {
            if(SPPile* rootPile = dynamic_cast<SPPile*>(parent->getPileRoot()))
            {
                if(rootPile->incrementing)
                {
                    bool suitsMatch = parentCard->getSuit() == childCard->getSuit();
                    bool valuesIncrease = (parentCard->getValue() + 1) == childCard->getValue();
                    if(suitsMatch && valuesIncrease)
                        return true;
                }
                else
                {
                    bool suitsAlternate = (parentCard->getSuit() % 2) != (childCard->getSuit() % 2);
                    bool valuesDecrease = parentCard->getValue() == childCard->getValue() + 1;
                    if(suitsAlternate && valuesDecrease)
                        return true;
                }
            }
        }
    }

    return false;
}