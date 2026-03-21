//
// Created by will on 8/17/25.
//

#include "NSScene.h"
#include "NSGameMode.h"

#include <iostream>
#include <ostream>

#include "InputManager.h"
#include "NSSelectAction.h"
#include "NSCancelAction.h"
#include "NSDealAction.h"
#include "NSResetAction.h"
#include "NSUndoAction.h"
#include "OptionsManager.h"
#include "SPGameplayAction.h"
#include "AnimationComponent.h"

NSGameMode::NSGameMode(NSGameState* gameState)
: gameState(gameState)
{
    // this.scene = scene;
    // gameStategameMode = this;

//    InputConfig config = InputConfig();
//    auto* inputComponent = new InputComponent(config);
//    addComponent(inputComponent);

    gameState->gameMode = this;

    selectAction = new NSSelectAction(this);
    cancelAction = new NSCancelAction(this);
    dealAction = new NSDealAction(this);
    undoAction = new NSUndoAction(this);
    resetAction = new NSResetAction(this);

    // TODO: Move all these input bindings to some external class with a save/load serialization option
    // Window Inputs
    InputManager::getInstance()->addBinding(KEY_ESCAPE, BUTTON_ACTION_PRESS, Event(Event::EVT_QUIT_GAME));
    InputManager::getInstance()->addBinding(KEY_F1, BUTTON_ACTION_PRESS, Event(Event::EVT_WIREFRAME_MODE));

    // Directional Input
    InputManager::getInstance()->addBinding(MOUSE_BUTTON_NONE, BUTTON_ACTION_NONE, Event(SPEvent::EVT_DRAG));
    InputManager::getInstance()->addBinding(GAMEPAD_BUTTON_DPAD_UP, BUTTON_ACTION_PRESS, Event(SPEvent::EVT_UP));
    InputManager::getInstance()->addBinding(GAMEPAD_BUTTON_DPAD_DOWN, BUTTON_ACTION_PRESS, Event(SPEvent::EVT_DOWN));
    InputManager::getInstance()->addBinding(GAMEPAD_BUTTON_DPAD_LEFT, BUTTON_ACTION_PRESS, Event(SPEvent::EVT_LEFT));
    InputManager::getInstance()->addBinding(GAMEPAD_BUTTON_DPAD_RIGHT, BUTTON_ACTION_PRESS, Event(SPEvent::EVT_RIGHT));

    // Press
    InputManager::getInstance()->addBinding(MOUSE_BUTTON_1, BUTTON_ACTION_PRESS, Event(SPEvent::EVT_PRESS));
    InputManager::getInstance()->addBinding(GAMEPAD_BUTTON_A, BUTTON_ACTION_PRESS, Event(SPEvent::EVT_PRESS));

    // Release
    InputManager::getInstance()->addBinding(MOUSE_BUTTON_1, BUTTON_ACTION_RELEASE, Event(SPEvent::EVT_RELEASE));
    InputManager::getInstance()->addBinding(GAMEPAD_BUTTON_A, BUTTON_ACTION_RELEASE, Event(SPEvent::EVT_RELEASE));

    // Cancel
    InputManager::getInstance()->addBinding(MOUSE_BUTTON_2, BUTTON_ACTION_RELEASE, Event(SPEvent::EVT_CANCEL));
    InputManager::getInstance()->addBinding(GAMEPAD_BUTTON_B, BUTTON_ACTION_RELEASE, Event(SPEvent::EVT_CANCEL));

    // Deal
    InputManager::getInstance()->addBinding(KEY_D, BUTTON_ACTION_PRESS, Event(SPEvent::EVT_DEAL));
    InputManager::getInstance()->addBinding(GAMEPAD_BUTTON_Y, BUTTON_ACTION_PRESS, Event(SPEvent::EVT_DEAL));

    // Undo
    InputManager::getInstance()->addBinding(KEY_Z, BUTTON_ACTION_PRESS, Event(SPEvent::EVT_UNDO), MOD_CONTROL);
    InputManager::getInstance()->addBinding(GAMEPAD_BUTTON_X, BUTTON_ACTION_PRESS, Event(SPEvent::EVT_UNDO));

    // Game Flow
    InputManager::getInstance()->addBinding(KEY_R, BUTTON_ACTION_PRESS, Event(SPEvent::EVT_RESET_GAME));



#ifdef PILEPEEKER
    std::string text = "0.0";
    Shader textShader = AssetManager::getInstance()->loadShader("shaders/text.vert",
                                                                "shaders/text.frag",
                                                                nullptr,
                                                                text);
    TextFont textFont = AssetManager::getInstance()->loadTextFont("assets/arial.ttf",
                                                                  "arial18", 18);
    auto* textComponent = new TextComponent(textShader, textFont, text);
    textComponent->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
    textComponent->setTransform(glm::vec3(0, -10.f, 0.001f));
    addComponent(textComponent);
#endif

    OptionsManager::getInstance()->registerReceiver(this);

    // this->transform.setPosition(glm::vec3(this->transform.getPosition().x, this->transform.getPosition().y, 0.5f));
    //
    // shouldUpdate = true;

    // gameState->unselectedUIGrid.setPriorityX(UIGD_PRIORITIZE_AXIS);
    // gameState->unselectedUIGrid.setPriorityY(UIGD_PRIORITIZE_ALIGNMENT);
    // gameState->unselectedUIGrid.wrapX = true;
    // gameState->unselectedUIGrid.wrapY = true;
    //
    // gameState->selectedUIGrid.setPriorityX(UIGD_PRIORITIZE_AXIS);
    // gameState->selectedUIGrid.setPriorityY(UIGD_PRIORITIZE_ALIGNMENT);
    // gameState->selectedUIGrid.wrapX = true;
    // gameState->selectedUIGrid.wrapY = true;
    //
    // gameState->activeUIGrid = &gameState->unselectedUIGrid;
}

NSGameMode::~NSGameMode()
{
    delete(selectAction);
    delete(cancelAction);
    delete(dealAction);
    delete(undoAction);
    OptionsManager::getInstance()->deregisterReceiver(this);
}

void NSGameMode::initialSetup(Scene *scene)
{
    bool easyDeal = false; // Debug tool to make it easier to play through the game
//    EntityManager* entityManager = EntityManager::getInstance();

    // gameState->owningScene = scene;

    // Two full decks of cards
    std::vector<CFCard*>* cardSet = &gameState->cards;
    for(int deckNum = 0; deckNum < numSets; deckNum++)
    {
//        for(int suit = SPSUIT_MIN; suit <= SPSUIT_MAX; suit++)
//        {
            for(int rank = CF_RANK_MIN; rank <= CF_RANK_MAX; rank++)
            {
//                SPCard* newCard = new SPCard(glm::vec2(10.f, 10.f), (CFCardSuit) suit, (CFCardRank) value, false, this);
//                SPCard* newCard = new SPCard();

//                entityManager->registerEntity(scene, newCard);
                auto* newCard = scene->addEntity<CFCard>(glm::vec2(10.f, 10.f), CF_SUIT_SPADE, static_cast<CFCardRank>(rank), false, gameState);
                cardSet->push_back(newCard);
            }
//        }
    }



    // std::shuffle(std::begin(*cardSet), std::end(gameState->cards), rng);
    // std::shuffle(std::begin(gameState->cards), std::end(gameState->cards), rng);
    // std::shuffle(std::begin(gameState->cards), std::end(gameState->cards), rng);
    // std::shuffle(std::begin(gameState->cards), std::end(gameState->cards), rng);
    // std::shuffle(std::begin(gameState->cards), std::end(gameState->cards), rng);
    // std::shuffle(std::begin(gameState->cards), std::end(gameState->cards), rng);
    // std::shuffle(std::begin(gameState->cards), std::end(gameState->cards), rng);

    for(int i = 0; i < numTableaus; i++)
    {
        auto* tableau = scene->addEntity<SPPile>(glm::vec2(10.f + (i * 55.f), 100.f),
                                  glm::vec3(0.0f, 0.0f, STACK_OFFSET),
                                  glm::vec3(0.0f, 20.0f, STACK_OFFSET));
        gameState->tableaus.push_back(tableau);
//        entityManager->registerEntity(scene, pile);
    }

    // Foundation piles
    for(int i = 0; i < numSets; i++)
    {
        auto* foundation = scene->addEntity<SPPile>(glm::vec2(100.f + (i * 55.f), 10.f),
                                  glm::vec3(0.0f, 0.0f, STACK_OFFSET),
                                  glm::vec3(0.0f, 0.0f, STACK_OFFSET));
        gameState->foundations.push_back(foundation);
    }

    // // Four face down rows
    // for(int i = 0; i < 4; i++)
    // {
    //     for (auto *tableau : gameState.tableaus)
    //     {
    //         tableau->addToPile(cardSet.back(), true);
    //         cardSet.pop_back();
    //     }
    // }

    // // Four more cards in the first four piles
    // for(int i = 0; i < 4; i++)
    // {
    //     gameState.tableaus[i]->addToPile(cardSet.back(), true);
    //     cardSet.pop_back();
    // }

    // Add all cards to the stock
    gameState->stock = scene->addEntity<CFDeck>(glm::vec2(10.f, 10.f), .125f);
    for (auto* card : gameState->cards) {
        gameState->stock->addToPile(card, false, "loadStock");
    }
    if(!easyDeal)
        gameState->stock->shuffle();

    // Deal face down cards into tableaus
    // if (!easyDeal) {
    //     gameState->stock->shuffle();
    // }

    updateLayout();

    deal();

    // One face up card in each pile
    // for(auto* tableau : gameState->tableaus)
    // {
    //     auto* card = reinterpret_cast<CFCard*>(gameState->stock->getPileEnd());
    //     tableau->addToPile(card, true);
    //     card->flip();
    // }



    // gameState->cursor = scene->addEntity<SPCursor>(gameState->tableaus[0]->getPileEnd(), glm::vec2(5.0f, 5.0f));
    // gameState->cursor->enable();
//    entityManager->registerEntity(scene, gameState.cursor);


    // gameState->stock->deal(gameState->tableaus[0]);
    // gameState->updateUnselectedUIGrid();
}

void NSGameMode::deal()
{
    // Classic deal
    int cardsToDeal = 5 * numTableaus + 4;
    for(int i = 0; i < cardsToDeal; i++)
    {
        CFPile* currTableau = gameState->tableaus[i%numTableaus];
        gameState->stock->deal(currTableau);
    }

    // for (int i = 0; i < gameState->tableaus.size(); i++)
    // {
    //     if (easyDeal)
    //     {
    //         for (int j = gameState->tableaus.size() - i - 1; j < gameState->tableaus.size(); j++)
    //         {
    //             gameState->stock->deal(gameState->tableaus[j]);
    //         }
    //     }
    //     else
    //     {
    //         for (int j = i; j < gameState->tableaus.size(); j++)
    //         {
    //             // IAnimationCompleteReceiver* receiver = nullptr;
    //             // AnimCompleteFunction callback = nullptr;
    //             //
    //             // if(i == gameState->tableaus.size() - 1 && j == gameState->tableaus.size() - 1)
    //             // {
    //             //     receiver = this;
    //             //     callback = static_cast<AnimCompleteFunction>(&NSGameMode::dealComplete);
    //             // }
    //
    //             gameState->stock->deal(gameState->tableaus[j]);
    //         }
    //     }
    //     // if (easyDeal) {
    //     //     for (int j = gameState->tableaus.size() - i - 1; j < gameState->tableaus.size(); j++) {
    //     //         auto* card = reinterpret_cast<CFCard*>(gameState->stock->getPileEnd());
    //     //         gameState->tableaus[j]->addToPile(card, true);
    //     //         if (i == gameState->tableaus.size() - 1)
    //     //             card->flip();
    //     //     }
    //     // }
    //     // else {
    //     //     for (int j = i; j < gameState->tableaus.size(); j++) {
    //     //         auto* card = reinterpret_cast<CFCard*>(gameState->stock->getPileEnd());
    //     //         gameState->tableaus[j]->addToPile(card, true);
    //     //         if (j == i)
    //     //             card->flip();
    //     //     }
    //     // }
    // }
}

void NSGameMode::dealComplete(std::string identifier)
{
    for (int i = 0; i < gameState->tableaus.size(); i++)
    {
        CFCard* card = dynamic_cast<CFCard*>(gameState->tableaus[i]->getPileEnd());
        if (card->isFaceDown())
            card->flip();
        rescalePile(gameState->tableaus[i]);
    }
}

void NSGameMode::updateLayout()
{
    glm::vec2 viewportResolution = OptionsManager::getInstance()->getViewportResolution();

    glm::vec2 initialSize = glm::vec2(49.f, 64.f); // TODO: Find a way to not hard code this

    float cardSizeRatio = initialSize.x / initialSize.y;

    float targetWidth = viewportResolution.x / (numTableaus + 1); // numTableaus plus one cards worth of space spread out between
    float gapWidth = targetWidth / (numTableaus + 1);

    float targetHeight = targetWidth / cardSizeRatio;

    // Tableaus
    float currX = gapWidth;
    for(CFPile* tableau : gameState->tableaus)
    {
        tableau->getTransform()->setPosition2(glm::vec2(currX, targetHeight + (2 * gapWidth)));

        glm::vec2 prevSize = tableau->getSize();
        float scaleMultiplier = targetWidth/initialSize.x;
        tableau->setSize(glm::vec2(initialSize.x * scaleMultiplier, initialSize.y * scaleMultiplier));

        tableau->setPileOffset(glm::vec3(0.0f, 20.f * targetHeight/initialSize.y, STACK_OFFSET));
        tableau->setInitialPileOffset(tableau->getPileOffset());

        for(CFCard* currCard = dynamic_cast<CFCard*>(tableau->getPileChild());
            currCard != nullptr;
            currCard = dynamic_cast<CFCard*>(currCard->getPileChild()))
        {
            // Card Size
            glm::vec2 prevSize = currCard->getSize();
            float scaleMultiplier = targetWidth/initialSize.x;
            currCard->setSize(glm::vec2(initialSize.x * scaleMultiplier, initialSize.y * scaleMultiplier));

            currCard->setPileOffset(glm::vec3(0.0f, 20.f * targetHeight/initialSize.y, STACK_OFFSET));
            currCard->setInitialPileOffset(currCard->getPileOffset());

            if(currCard->getPileParent() == tableau)
                currCard->snapTo(currCard->getPileParent()->getRootOffset());
            else
                currCard->snapTo(currCard->getPileParent()->getPileOffset());
        }

        currX += targetWidth + gapWidth;
    }

    // Foundations
    currX = gapWidth * 2 + targetWidth * 1.5;
    float outPileGapWidth = ((viewportResolution.x - currX) - (numSets * targetWidth)) / numSets;
    for(CFPile* foundation : gameState->foundations)
    {
        foundation->getTransform()->setPosition2(glm::vec2(currX, gapWidth));

        glm::vec2 prevSize = foundation->getSize();
        float scaleMultiplier = targetWidth/initialSize.x;
        foundation->setSize(glm::vec2(initialSize.x * scaleMultiplier, initialSize.y * scaleMultiplier));

        for(CFCard* currCard = dynamic_cast<CFCard*>(foundation->getPileChild());
            currCard != nullptr;
            currCard = dynamic_cast<CFCard*>(currCard->getPileChild()))
        {
            // Card Size
            glm::vec2 prevSize = currCard->getSize();
            float scaleMultiplier = targetWidth/initialSize.x;
            currCard->setSize(glm::vec2(initialSize.x * scaleMultiplier, initialSize.y * scaleMultiplier));
        }

        currX += targetWidth + outPileGapWidth;
    }

    // Stock
    float scaleMultiplier = targetWidth/initialSize.x;
    gameState->stock->getTransform()->setPosition2(glm::vec2(gapWidth, gapWidth));
    gameState->stock->setSize(glm::vec2(initialSize.x * scaleMultiplier, initialSize.y * scaleMultiplier));

    for(CFCard* currCard = dynamic_cast<CFCard*>(gameState->stock->getPileChild());
        currCard != nullptr;
        currCard = dynamic_cast<CFCard*>(currCard->getPileChild()))
    {
        // Card Size
        currCard->setSize(glm::vec2(initialSize.x * scaleMultiplier, initialSize.y * scaleMultiplier));
    }

    for(CFPile* tableau : gameState->tableaus)
    {
        rescalePile(tableau);
    }
}

void NSGameMode::cardAnimationComplete(CFCard* card, std::string identifier)
{
    if(card->isSelected())
        card->deselect();

    // TODO: Rescale just the two affected piles
    for(auto* tableau : gameState->tableaus)
        rescalePile(tableau);

    // All piles filled, you win!
    // TODO: Maybe it's worth checking if the parent is an outpile too to optimize this?
    if(dynamic_cast<CFCard*>(card)->getRank() == CF_RANK_KING)
    {
        bool winDetected = gameState->foundations.size() == numSets;

        for(int currFoundation = 0; currFoundation < gameState->foundations.size(); currFoundation++)
        {
            if (!gameState->foundations[currFoundation]->getPileChild()) {
                winDetected = false;
                break;
            }
        }

        if(winDetected)
            EventManager::getInstance()->broadcastEvent(Event(Event::EVT_WON_GAME));
    }

    for(auto foundation : gameState->foundations)
        if(card->getPileRoot() == foundation)
            return;

    // if(card->getPileRoot() == gameState->stock)
    // {
    //     dynamic_cast<CFCard *>(card)->flip();
    //     return;
    // }

    handleCompleteSuitIfFound(card);
}

void NSGameMode::handleCompleteSuitIfFound(CFPilable *pilable)
{
    if(CFCard* childCard = dynamic_cast<CFCard*>(pilable))
    {
        int currSuit = childCard->getSuit();
        CFCard* topCard = nullptr;

        CFCard* prevCard = dynamic_cast<CFCard*>(childCard->getPileParent());
        while(prevCard)
        {
            if(prevCard->getSuit() != currSuit)
                return;

            if(prevCard->isFaceDown())
                return;

            if(prevCard->getRank() == CF_RANK_KING)
            {
                topCard = prevCard;
                break;
            }

            prevCard = dynamic_cast<CFCard*>(prevCard->getPileParent());
        }

        if(!topCard)
            return;

        // PrevCard is a suited king
        int targetValue = CF_RANK_KING - 1;
        CFCard* currCard = dynamic_cast<CFCard*>(topCard->getPileChild());
        while(currCard)
        {
            if(currCard->getRank() != targetValue)
                return;

            if(currCard->getSuit() != currSuit)
                return;

            // Cards are still moving, wait until everything is settled
            if(currCard != pilable && currCard->hasUnfinishedAnimations())
                return;

            if(currCard->getRank() == CF_RANK_ACE)
                break;

            currCard = dynamic_cast<CFCard*>(currCard->getPileChild());
            targetValue = targetValue - 1;
        }

        if(currCard == nullptr)
            return;

        // If we get here, we have a King through Ace suited
        gameState->moveList.push_back(MoveEntry(topCard->getPileParent(), topCard));

        if(CFCard* parentCard = dynamic_cast<CFCard*>(topCard->getPileParent()))
            if(parentCard->isFaceDown())
                parentCard->flip();

        int currFoundation = 0;
        while(gameState->foundations[currFoundation]->getPileChild())
            currFoundation++;

        // gameState->cursor->setTarget(topCard->getPileParent());

        topCard->removeFromPile();
        topCard->raiseToFront();
        gameState->foundations[currFoundation]->addToPile(topCard, false);//, this);
        currFoundation++;

        // gameState->updateUnselectedUIGrid();
    }
}

void NSGameMode::rescalePile(CFPilable* pilable)
{
    if(!pilable)
        return;

    if(pilable->getPileRoot() == gameState->stock)
        return;

    for(auto* foundation : gameState->foundations)
        if(pilable->getPileRoot() == foundation)
            return;

    // Don't mess with piles that have cards moving
    for (CFPilable *currPilable = pilable->getPileRoot();
         currPilable != nullptr; currPilable = currPilable->getPileChild())
    {
        if(AnimationComponent* animComp = currPilable->getComponent<AnimationComponent>())
        {
            if(animComp->hasUnfinishedAnimations())
            {
                return;
            }
        }
    }

    glm::vec2 viewportRes = OptionsManager::getInstance()->getViewportResolution();

    // Reset pile to initial offset
    if(pilable->getPileOffset() != pilable->getInitialPileOffset())
    {
        for (CFPilable *currPilable = pilable->getPileRoot();
             currPilable != nullptr; currPilable = currPilable->getPileChild())
        {
            currPilable->setPileOffset(currPilable->getInitialPileOffset());
            if(currPilable != currPilable->getPileRoot() &&
               currPilable->getPileParent() != currPilable->getPileRoot())
                currPilable->snapTo(currPilable->getPileParent()->getPileOffset());
        }
    }

    // Crush pile down if it doesn't fit
    while(pilable->getPileEnd()->getWorldTransform().getPosition().y + pilable->getPileEnd()->getSize().y > viewportRes.y - (pilable->getSize().y / 11))
    {
        for(CFPilable* currPilable = pilable->getPileRoot(); currPilable != nullptr; currPilable = currPilable->getPileChild())
        {
            glm::vec3 offset = currPilable->getPileOffset();
            offset.y -= 1;
            currPilable->setPileOffset(offset);
            if(currPilable != currPilable->getPileRoot() &&
               currPilable->getPileParent() != currPilable->getPileRoot())
                currPilable->snapTo(currPilable->getPileParent()->getPileOffset());
        }
    }
}

void NSGameMode::resetGame()
{
    // Don't allow reset while dealing
    if(gameState->stock->isDealing())
        return;

    bool signaled = false;
    for(CFCard* card : gameState->cards)
    {
        if(card->isFaceUp())
            card->flip();

        if(card->getPileRoot() != gameState->stock)
        {
            if(signaled)
                gameState->stock->addToPile(card);
            else
            {
                signaled = true;
                gameState->stock->getPileEnd()->addToPile(card, false, "resetGameComplete");
            }
        }
    }

    gameState->moveList.clear();
}

void NSGameMode::resetGameComplete()
{
    gameState->stock->shuffle();
    deal();
}

void NSGameMode::resolutionUpdated(glm::vec2 oldRes, glm::vec2 newRes)
{
    updateLayout();
}
