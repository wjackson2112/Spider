//
// Created by Will on 1/14/2023.
//

#ifndef SP_SNAP_VALIDATOR_FOUR_SUITS_H
#define SP_SNAP_VALIDATOR_FOUR_SUITS_H

#include "SPSnapValidator.h"

class SPSnapValidatorFourSuits : public SPSnapValidator
{
public:
    bool validate(Entity* parent, Entity* child);
};

#endif //SP_SNAP_VALIDATOR_FOUR_SUITS_H
