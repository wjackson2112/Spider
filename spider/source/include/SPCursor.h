//  Copyright (c) 2026 William Jackson
//  Licensed under the MIT License

//
// Created by Will on 3/31/2023.
//

#ifndef SP_CURSOR_H
#define SP_CURSOR_H

#include "Entity.h"

class SPCursor : public Entity
{
    bool enabled = false;

public:

    SPCursor(Entity* initialTarget, glm::vec2 initialOffset);
    Entity* getTarget() { return parent; }
    void setTarget(Entity* newTarget) { this->setParent(newTarget); }

    glm::vec3 getOffset() { return transform.getPosition(); }
    void setOffset(glm::vec2 offset) { transform.setPosition(glm::vec3(offset.x, offset.y, 0.01f)); }

    void enable();
    void disable();
};

#endif //SP_CURSOR_H
