//
// Created by Will on 1/14/2023.
//

#ifndef SP_SNAP_VALIDATOR_FOUR_SUITS_H
#define SP_SNAP_VALIDATOR_FOUR_SUITS_H

#include <vector>

#include "Entity.h"
#include "IInputReceiver.h"
#include "SPSnapValidator.h"
#include "SPPile.h"

#include <random>

#define NO_GRAB glm::vec2(-1, -1)
#define DRAG_THRESHOLD 5

struct MoveEntry
{
    SPPilable* child;
    SPPilable* parent;
    bool parentFaceUp = true;

    MoveEntry(SPPilable* parent, SPPilable* child)
    : parent(parent)
    , child(child)
    {
        if(SPCard* parentCard = dynamic_cast<SPCard*>(parent))
            parentFaceUp = parentCard->isFaceUp();
    }
};

class SPSnapValidatorFourSuits : public SPSnapValidator, public Entity
{
    std::vector<MoveEntry> moveList;

    std::random_device rd = std::random_device {};
    std::default_random_engine rng = std::default_random_engine {rd()};

    std::vector<SPPile*> playPiles;
    std::vector<SPPile*> outPiles;
    SPPile* deck;

public:
    SPSnapValidatorFourSuits();

    void undo();

private:
    void deal();
    void undoDeal();

    SPCard* getTopmostCardAtPosition(glm::vec2 position);
    void update(float deltaTime);

public:
    //SPSnapValidator
    void initialSetup(Scene* scene);

    void reportClick(SPPilable* pilable);

    bool validateGrab(SPPilable* parent, SPPilable* child);
    void reportGrab(SPPilable* parent, SPPilable* child);

    bool validateRelease(SPPilable* parent, SPPilable* child);
    void reportRelease(SPPilable* parent, SPPilable* child);
};

#endif //SP_SNAP_VALIDATOR_FOUR_SUITS_H
