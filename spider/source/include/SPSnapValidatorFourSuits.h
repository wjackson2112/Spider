//
// Created by Will on 1/14/2023.
//

#ifndef SP_SNAP_VALIDATOR_FOUR_SUITS_H
#define SP_SNAP_VALIDATOR_FOUR_SUITS_H

#include <vector>

#include "IInputReceiver.h"
#include "SPSnapValidator.h"
#include "SPPile.h"

#include <random>

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

class SPSnapValidatorFourSuits : public SPSnapValidator, public IInputReceiver
{
    std::vector<MoveEntry> moveList;

    std::random_device rd = std::random_device {};
    std::default_random_engine rng = std::default_random_engine {rd()};

    std::vector<SPPile*> playPiles;
    std::vector<SPPile*> outPiles;
    SPPile* deck;

public:
    SPSnapValidatorFourSuits();
    ~SPSnapValidatorFourSuits();

    void undo();

private:
    void deal();
    void undoDeal();

public:
    //SPSnapValidator
    void initialSetup(Scene* scene);

    void reportClick(SPPilable* pilable);

    bool validateGrab(SPPilable* parent, SPPilable* child);
    void reportGrab(SPPilable* parent, SPPilable* child);

    bool validateRelease(SPPilable* parent, SPPilable* child);
    void reportRelease(SPPilable* parent, SPPilable* child);

    //IInputReceiver
    void mouseInputCallback(double xpos, double ypos, MouseButton button, Action action, Modifier mods) {};
    void keyInputCallback(Key key, int scancode, Action action, Modifier mods);
};

#endif //SP_SNAP_VALIDATOR_FOUR_SUITS_H
