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
#include "NSUndoAction.h"

class NSScene;

class NSGameMode : public CFGameMode, public IOptionsReceiver
{
    int numSets = 4;
    int numTableaus = 7;

    std::random_device rd = std::random_device {};
    std::default_random_engine rng = std::default_random_engine {rd()};

    // NSGameState gameState;


    NSSelectAction* selectAction;
    NSCancelAction* cancelAction;
    NSDealAction* dealAction;
    NSUndoAction* undoAction;

public:
    NSGameState* gameState;
    NSGameMode(NSGameState* gameState);
    ~NSGameMode() override;

public:
    //SPSnapValidator
    void initialSetup(Scene* scene) override;
    void updateLayout() override;

    void animationCompleteWithId(std::string identifier, Entity* entity) override;

    void handleCompleteSuitIfFound(CFPilable *pilable);
    void rescalePile(CFPilable* pilable);

    //IOptionsReceiver
    void resolutionUpdated(glm::vec2 oldRes, glm::vec2 newRes) override;
};

#endif //NS_GAME_MODE_H
