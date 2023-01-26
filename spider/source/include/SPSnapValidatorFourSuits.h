//
// Created by Will on 1/14/2023.
//

#ifndef SP_SNAP_VALIDATOR_FOUR_SUITS_H
#define SP_SNAP_VALIDATOR_FOUR_SUITS_H

#include "SPSnapValidator.h"
#include "SPPile.h"

#include <random>

class SPSnapValidatorFourSuits : public SPSnapValidator
{
    SPPilable* oldParent;

    std::random_device rd = std::random_device {};
    std::default_random_engine rng = std::default_random_engine {rd()};

    std::vector<SPPile*> playPiles;
    std::vector<SPPile*> outPiles;
    int currOutPile = 0;
    SPPile* deck;

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
