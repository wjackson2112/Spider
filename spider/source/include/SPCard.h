//
// Created by Will on 1/9/2023.
//

#ifndef SP_CARD_H
#define SP_CARD_H

#include <map>

#include "Entity.h"
#include "ICollisionReceiver.h"
#include "SPSnapValidator.h"
#include "SPPilable.h"

#define STACK_MIN 0.0f
#define STACK_MAX 0.1f

enum SPCardBackColor
{
    SPBACK_MIN,
    SPBACK_BLUE=SPBACK_MIN,
    SPBACK_RED,
    SPBACK_GREEN,
    SPBACK_PURPLE,
    SPBACK_MAX
};

enum SPCardSuit
{
    SPSUIT_MIN,
    SPSUIT_CLUB=SPSUIT_MIN,
    SPSUIT_DIAMOND,
    SPSUIT_SPADE,
    SPSUIT_HEART,
    SPSUIT_MAX
};

enum SPCardValue
{
    SPVALUE_MIN,
    SPVALUE_ACE=SPVALUE_MIN,
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
    SPVALUE_KING,
    SPVALUE_MAX
};

class SPCard : public SPPilable//, public ICollisionReceiver
{
    SPCardSuit suit;
    SPCardValue value;
    SPCardBackColor color;
    bool faceUp = true;

    SPSnapValidator* validator;

    glm::vec2 size;

    SPPilable* prevParent = nullptr;

    std::map<ICollisionReceiver*, glm::vec2> overlaps;

public:
    SPCard(glm::vec2 position, SPCardSuit suit, SPCardValue value, bool faceUp, SPSnapValidator* validator);
    ~SPCard();

    SPCardSuit getSuit() { return suit; }
    SPCardValue getValue() { return value; }

protected:
    void settleCard(); // Settle the card in a stack

public:
    SPPilable* getClosestOverlap();
    bool containsPoint(glm::vec2 point);
    bool isTopmostAtPoint(glm::vec2 point);
    bool isTopmost();

    void flip();
    bool isFaceUp() { return faceUp; }
    bool isFaceDown() { return !isFaceUp(); }

    //SPPilable
    glm::vec2 getSize() {return size;}
};

#endif //SP_CARD_H
