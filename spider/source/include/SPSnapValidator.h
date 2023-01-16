//
// Created by Will on 1/14/2023.
//

#ifndef SP_SNAP_VALIDATOR_H
#define SP_SNAP_VALIDATOR_H

#include "SPPilable.h"

class SPSnapValidator
{
public:
    virtual bool validate(SPPilable* parent, SPPilable* child) = 0;
};

#endif //SP_SNAP_VALIDATOR_H
