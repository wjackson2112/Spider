//
// Created by Will on 1/14/2023.
//

#include "SPSnapValidatorFourSuits.h"

#include <algorithm>

#include "SPCard.h"
#include "SPPile.h"
#include "EntityManager.h"
#include "EventManager.h"

#include "InputManager.h"
#include "InputComponent.h"

SPSnapValidatorFourSuits::SPSnapValidatorFourSuits()
{
    InputConfig config = InputConfig();
    config.mouseButtons.push_back(MOUSE_BUTTON_1);
    config.keys = {KEY_D, KEY_Z};
    config.receivesMousePosition = true;

    auto* inputComponent = new InputComponent(config);
    addComponent(inputComponent);

    receivesUpdates = true;
}

void SPSnapValidatorFourSuits::deal()
{
    for(auto* pile : playPiles)
        if(!pile->getPileChild())
            return;

    for(auto* pile : playPiles)
    {
        SPCard* card = dynamic_cast<SPCard*>(deck->getPileEnd());
        pile->addToPile(card);
        card->flip();
    }

    deck->getPileEnd()->receivesUpdates = true;
    moveList.push_back(MoveEntry(nullptr, nullptr));
}

void SPSnapValidatorFourSuits::undoDeal()
{
//    for(auto* pile : playPiles)
    for(std::vector<SPPile*>::reverse_iterator iter = playPiles.rbegin(); iter != playPiles.rend(); iter++)
    {
        SPPile* pile = *iter;
        SPCard* card = dynamic_cast<SPCard*>(pile->getPileEnd());
        card->removeFromPile();
        deck->addToPile(card);
        card->flip();
        deck->getPileEnd()->receivesUpdates = false;
    }

    deck->getPileEnd()->receivesUpdates = true;
}

void SPSnapValidatorFourSuits::initialSetup(Scene *scene)
{
    EntityManager* entityManager = EntityManager::getInstance();

    // Two full decks of cards
    std::vector<SPCard*> cardSet;
    for(int deckNum = 0; deckNum < 8; deckNum++)
    {
//        for(int suit = SPSUIT_MIN; suit < SPSUIT_MAX; suit++)
//        {
            for(int value = SPVALUE_MIN; value < SPVALUE_MAX; value++)
            {
//                SPCard* newCard = new SPCard(glm::vec2(10.f, 10.f), (SPCardSuit) suit, (SPCardValue) value, false, this);
                SPCard* newCard = new SPCard(glm::vec2(10.f, 10.f), SPSUIT_SPADE, (SPCardValue) value, false, this);
                entityManager->registerEntity(scene, newCard);
                cardSet.push_back(newCard);
            }
//        }
    }
    std::shuffle(std::begin(cardSet), std::end(cardSet), rng);
    std::shuffle(std::begin(cardSet), std::end(cardSet), rng);
    std::shuffle(std::begin(cardSet), std::end(cardSet), rng);
    std::shuffle(std::begin(cardSet), std::end(cardSet), rng);
    std::shuffle(std::begin(cardSet), std::end(cardSet), rng);
    std::shuffle(std::begin(cardSet), std::end(cardSet), rng);
    std::shuffle(std::begin(cardSet), std::end(cardSet), rng);

    for(int i = 0; i < 10; i++)
    {
        SPPile* pile = new SPPile(glm::vec2(10.f + (i * 55.f), 100.f),
                                  glm::vec3(0.0f, 0.0f, STACK_OFFSET),
                                  glm::vec3(0.0f, 20.0f, STACK_OFFSET),
                                  false, this);
        playPiles.push_back(pile);
        entityManager->registerEntity(scene, pile);
    }

    // Four face down rows
    for(int i = 0; i < 4; i++)
    {
        for (auto *pile : playPiles)
        {
            pile->addToPile(cardSet.back());
            cardSet.pop_back();
        }
    }

    // Four more cards in the first four piles
    for(int i = 0; i < 4; i++)
    {
        playPiles[i]->addToPile(cardSet.back());
        cardSet.pop_back();
    }

    // One face up card in each pile
    for(auto* pile : playPiles)
    {
        SPCard* card = cardSet.back();
        cardSet.pop_back();
        pile->addToPile(card);
        card->flip();
    }

    // 8 "out of play" piles
    for(int i = 0; i < 8; i++)
    {
        SPPile* pile = new SPPile(glm::vec2(100.f + (i * 55.f), 10.f),
                                  glm::vec3(0.0f, 0.0f, STACK_OFFSET),
                                  glm::vec3(0.0f, 0.0f, STACK_OFFSET),
                                  false, this);
        outPiles.push_back(pile);
        entityManager->registerEntity(scene, pile);
    }

    // Put the rest of the cards into the deck
    deck = new SPPile(glm::vec2(10.f, 10.f),
                      glm::vec3(0.0f, 0.0f, STACK_OFFSET),
                      glm::vec3(0.0f, 0.0f, STACK_OFFSET),
                      false, this);

    while(!cardSet.empty())
    {
        deck->addToPile(cardSet.back());
        cardSet.pop_back();
    }

    deck->getPileEnd()->receivesUpdates = true;
}

void SPSnapValidatorFourSuits::reportClick(SPPilable* pilable)
{
    if(pilable->getPileRoot() == deck)
    {
        deal();
    }
}

bool SPSnapValidatorFourSuits::validateGrab(SPPilable *parent, SPPilable *child)
{
    if(child->getPileRoot() == deck)
        return false;

    // Nothing below this card, legal
    if(child == child->getPileEnd())
        return true;

    if(SPCard* childCard = dynamic_cast<SPCard*>(child))
    {
        SPCardSuit targetSuit = childCard->getSuit();
        SPCardValue targetValue = (SPCardValue) (childCard->getValue() - 1);
        SPPilable* curr = childCard->getPileChild();
        while(curr != nullptr)
        {
            if(SPCard* currCard = dynamic_cast<SPCard*>(curr))
            {
                if(currCard->getSuit() != targetSuit)
                    return false;

                if(currCard->getValue() != targetValue)
                    return false;
            }
            targetValue = (SPCardValue) (targetValue - 1);
            curr = curr->getPileChild();
        }

        // Got to the bottom of the stack with everything in the same suit, legal
        return true;
    }
    else
    {
        return false;
    }
}

void SPSnapValidatorFourSuits::reportGrab(SPPilable *parent, SPPilable *child)
{
//    oldParent = parent;

    moveList.push_back(MoveEntry(parent, child));
}

bool SPSnapValidatorFourSuits::validateRelease(SPPilable* parent, SPPilable* child)
{
    if(SPCard* childCard = dynamic_cast<SPCard*>(child))
    {
        if(SPPile* parentPile = dynamic_cast<SPPile*>(parent))
        {
            if(parentPile->incrementing && childCard->getValue() == SPVALUE_ACE)
                return true;
            else if(!parentPile->incrementing)
                return true;
            return false;
        }

        if(SPCard* parentCard = dynamic_cast<SPCard*>(parent))
        {
            if(parentCard->isFaceDown())
                return false;

            if(SPPile* rootPile = dynamic_cast<SPPile*>(parent->getPileRoot()))
            {
                // Never legal to play on an out pile
                for(auto* outPile : outPiles)
                    if(rootPile == outPile)
                        return false;

                // Never legal to play on the deck
                if(rootPile == deck)
                    return false;

                // Remaining piles are play piles
                if(!rootPile->incrementing)
                {
//                    bool suitsAlternate = (parentCard->getSuit() % 2) != (childCard->getSuit() % 2);
                    bool valuesDecrease = parentCard->getValue() == childCard->getValue() + 1;
                    if(valuesDecrease)
                        return true;
                }
            }
        }
    }

    return false;
}

void SPSnapValidatorFourSuits::reportRelease(SPPilable *parent, SPPilable *child)
{
    // Flip face down cards in play when their child is removed
    if(SPCard* oldParentCard = dynamic_cast<SPCard*>(moveList.back().parent))
    {
        // Check that the player didn't put the card back where it came from
        if(parent != oldParentCard && oldParentCard->isFaceDown())
        {
            oldParentCard->flip();
        }

        if(moveList.back().child && moveList.back().parent &&
           moveList.back().child == child &&
           moveList.back().parent == parent)
        {
            // Forget the previous move, player put the card back
            moveList.pop_back();
            return;
        }
    }

    if(SPCard* childCard = dynamic_cast<SPCard*>(child))
    {
        int currSuit = childCard->getSuit();
        SPCard* topCard = nullptr;

        SPCard* prevCard = dynamic_cast<SPCard*>(childCard->getPileParent());
        while(prevCard)
        {
            if(prevCard->getSuit() != currSuit)
                return;

            if(prevCard->isFaceDown())
                return;

            if(prevCard->getValue() == SPVALUE_KING)
            {
                topCard = prevCard;
                break;
            }

            prevCard = dynamic_cast<SPCard*>(prevCard->getPileParent());
        }

        if(!topCard)
            return;

        // PrevCard is a suited king
        int targetValue = SPVALUE_KING - 1;
        SPCard* currCard = dynamic_cast<SPCard*>(topCard->getPileChild());
        while(currCard)
        {
            if(currCard->getValue() != targetValue)
                return;

            if(currCard->getSuit() != currSuit)
                return;

            if(currCard->getValue() == SPVALUE_ACE)
                break;

            currCard = dynamic_cast<SPCard*>(currCard->getPileChild());
            targetValue = targetValue - 1;
        }

        if(currCard == nullptr)
            return;

        // If we get here, we have a King through Ace suited
        if(SPCard* parentCard = dynamic_cast<SPCard*>(topCard->getPileParent()))
            if(parentCard->isFaceDown())
                parentCard->flip();

        moveList.push_back(MoveEntry(topCard->getPileParent(), topCard));

        int currOutPile = 0;
        while(outPiles[currOutPile]->getPileChild())
            currOutPile++;

        outPiles[currOutPile]->addToPile(topCard);

        // Stop receiving updates for cards out of play
        currCard = dynamic_cast<SPCard*>(outPiles[currOutPile]->getPileChild());
        while(currCard)
        {
            currCard->receivesUpdates = false;
            currCard = dynamic_cast<SPCard*>(currCard->getPileChild());
        }

        currOutPile++;

        // All piles filled, you win!
        if(currOutPile >= 8)
            EventManager::getInstance()->broadcastEvent(WON_GAME);
    }
}

void SPSnapValidatorFourSuits::undo()
{
    if(moveList.size() <= 0)
        return;


    MoveEntry currentEntry = moveList.back(); moveList.pop_back();

    // If both are nullptr, this move was a deal
    if(currentEntry.parent == nullptr && currentEntry.child == nullptr)
    {
        undoDeal();
        return;
    }

    bool isSuitMove = false;
    if(currentEntry.child->getPileParent())
    {
        // Undo'ing a suited stack to the foundation
        for(SPPile* outPile : outPiles)
            if(currentEntry.child->getPileParent() == outPile)
                isSuitMove = true;
        currentEntry.child->removeFromPile();
    }

    currentEntry.parent->addToPile(currentEntry.child);

    if(SPCard* parentCard = dynamic_cast<SPCard*>(currentEntry.parent))
        if(currentEntry.parentFaceUp != parentCard->isFaceUp())
            parentCard->flip();

    // Ensure all moved cards are receiving updates
    SPCard* currCard = dynamic_cast<SPCard*>(currentEntry.child);
    while(currCard)
    {
        currCard->receivesUpdates = true;
        currCard = dynamic_cast<SPCard*>(currCard->getPileChild());
    }

    if(isSuitMove)
        undo();
}

void SPSnapValidatorFourSuits::update(float deltaTime)
{
    Entity::update(deltaTime);

    auto* inputComponent = getComponent<InputComponent>();

    while(inputComponent->hasEvents())
    {
        InputEvent event = inputComponent->dequeueEvent();
        if(event.isKeyEvent())
        {
            switch(event.key)
            {
                case KEY_D:
                    if(event.action == ACTION_PRESS)
                        deal();
                case KEY_Z:
                    if(event.action == ACTION_PRESS && event.mods == MOD_CONTROL)
                        undo();
                default:
                    return;
            }
        }
        else
        {
            switch(event.button)
            {
                default:
                    return;
            }
        }
    }
}