//
// Created by Will on 1/9/2023.
//

#ifndef SP_CARD_H
#define SP_CARD_H

#include <map>

#include "Entity.h"
#include "IAnimationCompleteReceiver.h"
#include "SPSnapValidator.h"
#include "SPPilable.h"

enum SPCardBackColor
{
    SPBACK_MIN,
    SPBACK_BLUE=SPBACK_MIN,
    SPBACK_RED,
    SPBACK_GREEN,
    SPBACK_PURPLE,
    SPBACK_MAX=SPBACK_PURPLE
};

enum SPCardSuit
{
    SPSUIT_MIN,
    SPSUIT_CLUB=SPSUIT_MIN,
    SPSUIT_DIAMOND,
    SPSUIT_SPADE,
    SPSUIT_HEART,
    SPSUIT_MAX=SPSUIT_HEART
};

const std::string cardSuitStrings[6] = {
        "clubs",
        "diamonds",
        "spades",
        "hearts"
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
    SPVALUE_MAX=SPVALUE_KING
};

const std::string cardValueStrings[15] = {
        "ace",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9",
        "10",
        "jack",
        "queen",
        "king"
};

class SPCard : public SPPilable, public IAnimationCompleteReceiver
{
    SPCardSuit suit;
    SPCardValue value;
    SPCardBackColor color;
    bool faceUp = true;

    SPSnapValidator* validator;

    glm::vec2 size;

public:
    SPCard(glm::vec2 position, SPCardSuit suit, SPCardValue value, bool faceUp, SPSnapValidator* validator);
    ~SPCard();

    SPCardSuit getSuit() { return suit; }
    SPCardValue getValue() { return value; }

    SPPilable* getClosestOverlap();
    bool containsPoint(glm::vec2 point);
    bool isTopmostAtPoint(glm::vec2 point);
    bool isTopmost();

    void flip();
    bool isFaceUp() { return faceUp; }
    bool isFaceDown() { return !isFaceUp(); }
    bool hasUnfinishedAnimations();

    void select();
    void deselect();

    //SPPilable
    void moveTo(glm::vec3 target);
    void setSize(glm::vec2 size);
    glm::vec2 getSize() { return size; }

    //IAnimationCompleteReceiver
    void animationComplete(Entity* entity);
};

#endif //SP_CARD_H
