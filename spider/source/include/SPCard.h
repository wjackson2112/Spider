//
// Created by Will on 1/9/2023.
//

#ifndef SP_CARD_H
#define SP_CARD_H

#include <map>

#include "Entity.h"
#include "IAnimationCompleteReceiver.h"
//#include "SPSnapValidatorFourSuits.h"
#include "SPPilable.h"
#include "CFCard.h"

class SPGameState;

// enum SPCardBackColor
// {
//     SPBACK_MIN,
//     SPBACK_BLUE=SPBACK_MIN,
//     SPBACK_RED,
//     SPBACK_GREEN,
//     SPBACK_PURPLE,
//     SPBACK_MAX=SPBACK_PURPLE
// };
//
// enum SPCardSuit
// {
//     SPSUIT_MIN,
//     SPSUIT_CLUB=SPSUIT_MIN,
//     SPSUIT_DIAMOND,
//     SPSUIT_SPADE,
//     SPSUIT_HEART,
//     SPSUIT_MAX=SPSUIT_HEART
// };
//
// const std::string cardSuitStrings[6] = {
//         "clubs",
//         "diamonds",
//         "spades",
//         "hearts"
// };
//
// enum SPCardValue
// {
//     SPVALUE_MIN,
//     SPVALUE_ACE=SPVALUE_MIN,
//     SPVALUE_TWO,
//     SPVALUE_THREE,
//     SPVALUE_FOUR,
//     SPVALUE_FIVE,
//     SPVALUE_SIX,
//     SPVALUE_SEVEN,
//     SPVALUE_EIGHT,
//     SPVALUE_NINE,
//     SPVALUE_TEN,
//     SPVALUE_JACK,
//     SPVALUE_QUEEN,
//     SPVALUE_KING,
//     SPVALUE_MAX=SPVALUE_KING
// };
//
// const std::string cardValueStrings[15] = {
//         "ace",
//         "2",
//         "3",
//         "4",
//         "5",
//         "6",
//         "7",
//         "8",
//         "9",
//         "10",
//         "jack",
//         "queen",
//         "king"
// };

class SPCard : public CFCard
{
     using CFCard::CFCard;
//     CFCardSuit suit;
//     CFCardRank value;
//     CFCardBackColor color;
//     bool faceUp = true;
//
//     SPGameState* gameState;
//
//     glm::vec2 size;
//
// public:
     // SPCard(glm::vec2 position, CFCardSuit suit, CFCardRank value, bool faceUp, SPGameState* gameState);
//     ~SPCard();
//
//     CFCardSuit getSuit() { return suit; }
//     CFCardRank getValue() { return value; }
//
//     SPPilable* getClosestOverlap();
//     bool containsPoint(glm::vec2 point);
//     bool isTopmostAtPoint(glm::vec2 point);
//     bool isTopmost();
//
//     void flip();
//     bool isFaceUp() { return faceUp; }
//     bool isFaceDown() { return !isFaceUp(); }
//     bool hasUnfinishedAnimations();
//
//     void select();
//     void deselect();
//
//     //SPPilable
//     void moveTo(glm::vec3 target);
//     void setSize(glm::vec2 size);
//     glm::vec2 getSize() { return size; }
//
//     //IAnimationCompleteReceiver
//     void animationCompleteWithId(std::string identifier, Entity* entity);
};

#endif //SP_CARD_H
