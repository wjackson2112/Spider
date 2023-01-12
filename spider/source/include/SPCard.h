//
// Created by Will on 1/9/2023.
//

#ifndef SP_CARD_H
#define SP_CARD_H

#include "Entity.h"
#include "IInputReceiver.h"

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

class SPCard : public Entity, IInputReceiver
{
    SPCardSuit suit;
    SPCardValue value;

    glm::vec2 size;
    glm::vec2 grabPosition = NO_GRAB;
    glm::vec2 grabOffset = NO_GRAB;

public:
    SPCard(glm::vec2 position, SPCardSuit suit, SPCardValue value);

    SPCardSuit getSuit() { return suit; }
    SPCardValue getValue() { return value; }

    void update(float deltaTime);

    // IInputReceiver
    virtual void mouseInputCallback(double xpos, double ypos, MouseButton button, Action action, Modifier mods);
    virtual void keyInputCallback(Key key, int scancode, Action action, Modifier mods) {};
};

#endif //SP_CARD_H
