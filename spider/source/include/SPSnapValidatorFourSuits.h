//
// Created by Will on 1/14/2023.
//

#ifndef SP_SNAP_VALIDATOR_FOUR_SUITS_H
#define SP_SNAP_VALIDATOR_FOUR_SUITS_H

#include <vector>

#include "Entity.h"
#include "UIGrid.h"
#include "IInputReceiver.h"
#include "SPSnapValidator.h"
#include "SPPile.h"
#include "SPCursor.h"
#include "IOptionsReceiver.h"

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

enum InputMode
{
    IM_MKB,
    IM_GAMEPAD
};

struct SPGameState
{
    Scene* owningScene = nullptr;
    std::vector<SPPile*> tableaus;
    std::vector<SPPile*> foundations;
    SPPile* stock;

    SPCard* grabbedCard = nullptr;
    glm::vec2 grabStartPosition = NO_GRAB;
    glm::vec2 grabOffset = NO_GRAB;

    std::vector<MoveEntry> moveList;
};

class SPSnapValidatorFourSuits : public SPSnapValidator, public Entity, public IOptionsReceiver, public IAnimationCompleteReceiver
{
    std::random_device rd = std::random_device {};
    std::default_random_engine rng = std::default_random_engine {rd()};

    std::vector<SPCard*> ghostCards;

    SPGameState gameState;
    SPCursor* gamepadCursor;

    UIGrid unselectedUIGrid;
    UIGrid selectedUIGrid;
    UIGrid* activeUIGrid;

    InputMode inputMode = IM_MKB;

public:
    SPSnapValidatorFourSuits();
    ~SPSnapValidatorFourSuits();

    void undo();

private:
    void deal();
    void undoDeal();

    SPCard* getTopmostCardAtPosition(glm::vec2 position);
    void update(float deltaTime);

public:
    //SPSnapValidator
    void initialSetup(Scene* scene);
    void updateLayout();

    void reportClick(SPPilable* pilable);

//    bool validateGrab(SPPilable* parent, SPPilable* child);
//    void reportGrab(SPPilable* parent, SPPilable* child);

    bool validateRelease(SPPilable* parent, SPPilable* child);
    void reportRelease(SPPilable* parent, SPPilable* child);

    void reportAnimationComplete(SPPilable* pilable);

    void handleCompleteSuitIfFound(SPPilable *pilable);
    void rescalePile(SPPilable* pilable);
    void alignPile(SPPilable* pilable);

    //IOptionsReceiver
    void resolutionUpdated(glm::vec2 oldRes, glm::vec2 newRes);

private:
    void updateUnselectedUIGrid();
    void updateSelectedUIGrid(SPCard* selectedCard);
    void swapUIGrid();
    void updateGhostCards(SPCard* selectedCard);
    void clearGhostCards();
    void ghostCardClearComplete(Entity* card);
};

#endif //SP_SNAP_VALIDATOR_FOUR_SUITS_H
