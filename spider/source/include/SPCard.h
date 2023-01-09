//
// Created by Will on 1/9/2023.
//

#ifndef SP_CARD_H
#define SP_CARD_H

#include "Entity.h"

enum SPCardSuit
{
    SPSUIT_CLUB,
    SPSUIT_DIAMOND,
    SPSUIT_HEART,
    SPSUIT_SPADE
};

enum SPCardValue
{
    SPVALUE_ACE,
    SPVALUE_TWO,
    SPVALUE_THREE,
    SPVALUE_FOUR,
    SPVALUE_FIVE,
    SPVALUE_SIX,
    SPVALUE_SEVEN,
    SPVALUE_EIGHT,
    SPVALUE_NINE,
    SPVALUE_TEN,
    SPVALUE_JACK,
    SPVALUE_QUEEN,
    SPVALUE_KING
};

class SPCard : public Entity
{
public:
    SPCard();
};

#endif //SP_CARD_H
