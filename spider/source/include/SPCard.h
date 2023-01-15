//
// Created by Will on 1/9/2023.
//

#ifndef SP_CARD_H
#define SP_CARD_H

#include <map>

#include "Entity.h"
#include "ICollisionReceiver.h"

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

#define DRAG_THRESHOLD 5
#define NO_GRAB glm::vec2(-1, -1)

class SPCard : public Entity, public ICollisionReceiver
{
    SPCardSuit suit;
    SPCardValue value;

    glm::vec2 size;
    glm::vec2 grabPosition = NO_GRAB;
    glm::vec2 grabOffset = NO_GRAB;

    std::map<ICollisionReceiver*, glm::vec2> overlaps;

public:
    SPCard* snappedCard = nullptr;

    SPCard(glm::vec2 position, SPCardSuit suit, SPCardValue value);

    SPCardSuit getSuit() { return suit; }
    SPCardValue getValue() { return value; }

protected:
    void settleCard(); // Settle the card in a stack
    void snap(SPCard*);
    bool isInSnapPile(SPCard* other);

public:
    bool containsPoint(glm::vec2 point);
    bool isTopmostAtPoint(glm::vec2 point);
    bool isTopmost();


    //Entity
    void update(float deltaTime) override;

    //ICollisionReceiver
    void collisionCallback(ICollisionReceiver* collisionReceiver, glm::vec3 overlap);
};

#endif //SP_CARD_H
