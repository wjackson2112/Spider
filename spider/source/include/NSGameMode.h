//
// Created by will on 8/17/25.
//

#ifndef NS_GAME_MODE_H
#define NS_GAME_MODE_H

#include <random>

// #include "SPSnapValidatorFourSuits.h"
#include "CFGameMode.h"
#include "IOptionsReceiver.h"
#include "NSGameState.h"

#include "NSSelectAction.h"
#include "NSCancelAction.h"
#include "NSDealAction.h"
#include "NSResetAction.h"
#include "NSUndoAction.h"

class NSScene;
class NSGameState;

class NSGameMode : public CFGameMode, public IOptionsReceiver
{
    int numSets = 8;
    int numTableaus = 10;

    std::random_device rd = std::random_device {};
    std::default_random_engine rng = std::default_random_engine {rd()};

    // NSGameState gameState;


    NSSelectAction* selectAction;
    NSCancelAction* cancelAction;
    NSDealAction* dealAction;
    NSUndoAction* undoAction;
    NSResetAction* resetAction;

public:
    NSGameState* gameState;
    NSGameMode(NSGameState* gameState);
    ~NSGameMode() override;

public:
    //SPSnapValidator
    void initialSetup(Scene* scene) override;
    void deal();
    void dealComplete(std::string identifier);
    void updateLayout() override;

    void cardAnimationComplete(CFCard* card, std::string identifier) override;

    void handleCompleteSuitIfFound(CFPilable *pilable);
    void rescalePile(CFPilable* pilable);
    void resetGame();
    void resetGameComplete();

    //IOptionsReceiver
    void resolutionUpdated(glm::vec2 oldRes, glm::vec2 newRes) override;
};

#endif //NS_GAME_MODE_H
