//
// Created by Will on 1/21/2023.
//

#ifndef SP_FRAME_COUNTER_H
#define SP_FRAME_COUNTER_H

#include "Entity.h"

class SPFrameCounterEntity : public Entity
{
public:
    SPFrameCounterEntity();
    void update(float deltaTime);
};

#endif //SP_FRAME_COUNTER_H
