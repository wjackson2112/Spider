//  Copyright (c) 2026 William Jackson
//  Licensed under the MIT License

//
// Created by will on 8/20/25.
//

#ifndef NS_GAME_STATE_H
#define NS_GAME_STATE_H

#include "Scene.h"
#include "UIGrid.h"

#include "CFPile.h"
#include "CFCard.h"
#include "CFGameState.h"
#include "MoveEntry.h"

#include "SPCursor.h"

// #define NO_GRAB glm::vec2(-1, -1)
// #define DRAG_THRESHOLD 50

class NSGameMode;

struct NSGameState : public CFGameState
{
    // NSGameMode* gameMode = nullptr;
    std::vector<CFPile*> tableaus;
    std::vector<CFPile*> foundations;
    std::vector<CFCard*> ghostCards;
    CFDeck* stock;

    // SPCursor* cursor;

    // UIGrid unselectedUIGrid;
    // UIGrid selectedUIGrid;
    // UIGrid* activeUIGrid;

    CFCard* grabbedCard = nullptr;
    glm::vec2 grabStartPosition = NO_GRAB;
    glm::vec2 grabOffset = NO_GRAB;

    std::vector<MoveEntry> moveList;

    virtual ~NSGameState();

    void cardAnimationComplete(CFCard* card);



    // bool validateRelease(CFPilable* parent, CFPilable* child);

    // void updateUnselectedUIGrid();
    // void updateSelectedUIGrid(CFCard* selectedCard);
    // void swapUIGrid();
    // void updateGhostCards(CFCard* selectedCard);
    // void clearGhostCards();
    // void ghostCardClearComplete(Entity* card);
};

#endif //NS_GAME_STATE_H
