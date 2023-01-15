//
// Created by Will on 1/14/2023.
//

#ifndef SP_SNAP_VALIDATOR_H
#define SP_SNAP_VALIDATOR_H

#include "Entity.h"

class SPSnapValidator
{
public:
    virtual bool validate(Entity* parent, Entity* child) = 0;
};

#endif //SP_SNAP_VALIDATOR_H
