//
// Created by Will on 1/14/2023.
//

#ifndef SP_SNAP_VALIDATOR_FOUR_SUITS_H
#define SP_SNAP_VALIDATOR_FOUR_SUITS_H

#include <vector>

#include "Entity.h"
#include "UIGrid.h"
//#include "IInputReceiver.h"
#include "CFGameState.h"
#include "SPSnapValidator.h"
#include "SPPile.h"
#include "SPCursor.h"
#include "SPCard.h"
#include "IOptionsReceiver.h"

#include <random>

#define NO_GRAB glm::vec2(-1, -1)
#define DRAG_THRESHOLD 5

class SPSelectAction;
class SPCancelAction;
class SPDealAction;
class SPUndoAction;

struct MoveEntry
{
    CFPilable* child;
    CFPilable* parent;
    bool parentFaceUp = true;

    MoveEntry(CFPilable* parent, CFPilable* child)
    : parent(parent)
    , child(child)
    {
        if(SPCard* parentCard = dynamic_cast<SPCard*>(parent))
            parentFaceUp = parentCard->isFaceUp();
    }
};

class SPSnapValidatorFourSuits;

struct SPGameState : public CFGameState
{
    Scene* owningScene = nullptr;
    SPSnapValidatorFourSuits* gameMode = nullptr;
    std::vector<SPPile*> tableaus;
    std::vector<SPPile*> foundations;
    std::vector<SPCard*> ghostCards;
    SPPile* stock;

    SPCursor* cursor;

    UIGrid unselectedUIGrid;
    UIGrid selectedUIGrid;
    UIGrid* activeUIGrid;

    SPCard* grabbedCard = nullptr;
    glm::vec2 grabStartPosition = NO_GRAB;
    glm::vec2 grabOffset = NO_GRAB;

    std::vector<MoveEntry> moveList;

    SPSelectAction* selectAction;
    SPCancelAction* cancelAction;
    SPDealAction* dealAction;
    SPUndoAction* undoAction;

    void animationCompleteWithId(std::string identifier, Entity *entity);

    bool validateRelease(CFPilable* parent, CFPilable* child);
    //void reportRelease(SPPilable* parent, SPPilable* child);

    void updateUnselectedUIGrid();
    void updateSelectedUIGrid(SPCard* selectedCard);
    void swapUIGrid();
    void updateGhostCards(SPCard* selectedCard);
    void clearGhostCards();
    void ghostCardClearComplete(Entity* card);
};

class SPSnapValidatorFourSuits : public SPSnapValidator, public Entity, public IOptionsReceiver, public IAnimationCompleteReceiver
{
    std::random_device rd = std::random_device {};
    std::default_random_engine rng = std::default_random_engine {rd()};

    SPGameState gameState;

public:
    SPSnapValidatorFourSuits();
    ~SPSnapValidatorFourSuits();

//private:
//    void update(float deltaTime);

public:
    //SPSnapValidator
    void initialSetup(Scene* scene);
    void updateLayout();

    void animationCompleteWithId(std::string identifier, Entity* entity);

    void handleCompleteSuitIfFound(CFPilable *pilable);
    void rescalePile(CFPilable* pilable);

    //IOptionsReceiver
    void resolutionUpdated(glm::vec2 oldRes, glm::vec2 newRes);
};

#endif //SP_SNAP_VALIDATOR_FOUR_SUITS_H
