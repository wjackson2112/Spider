//
// Created by Will on 6/24/2022.
//

#ifndef SP_BACKGROUND_H
#define SP_BACKGROUND_H

#include <string>
#include "Entity.h"
#include "Texture2D.h"

#include "IOptionsReceiver.h"

class SPBackground : public Entity, public IOptionsReceiver
{
public:
    SPBackground();
    ~SPBackground();

    void resolutionUpdated(glm::vec2 oldRes, glm::vec2 newRes);
};

#endif //SP_BACKGROUND_H
