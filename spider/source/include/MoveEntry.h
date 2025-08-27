//
// Created by will on 8/20/25.
//

#ifndef MOVE_ENTRY_H
#define MOVE_ENTRY_H

#include "CFPilable.h"
#include "CFCard.h"

struct MoveEntry
{
    std::string type;
    int numCards = 0;
    CFPilable* child = nullptr;
    CFPilable* parent = nullptr;
    bool parentFaceUp = true;

    MoveEntry(CFPilable* parent, CFPilable* child)
    : type("MOVE")
    , parent(parent)
    , child(child)
    {
        if(CFCard* parentCard = dynamic_cast<CFCard*>(parent))
            parentFaceUp = parentCard->isFaceUp();
    }

    explicit MoveEntry(int numCards)
    : type("DEAL")
    , numCards(numCards) {}
};

#endif //MOVE_ENTRY_H
