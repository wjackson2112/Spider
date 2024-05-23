//
// Created by Will on 1/14/2023.
//

#ifndef SP_SNAP_VALIDATOR_H
#define SP_SNAP_VALIDATOR_H

#include "SPPilable.h"
#include "SPScene.h"

class SPSnapValidator
{
public:
    virtual void initialSetup(Scene* scene) {};

    virtual void reportAnimationComplete(SPPilable* pilable) {};
};

#endif //SP_SNAP_VALIDATOR_H
