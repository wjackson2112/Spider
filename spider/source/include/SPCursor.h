//
// Created by Will on 3/31/2023.
//

#ifndef SP_POINTER_H
#define SP_POINTER_H

#include "Entity.h"

class SPCursor : public Entity
{
public:
    SPCursor(Entity* initialTarget, glm::vec2 initialOffset);
    Entity* getTarget() { return parent; }
    void setTarget(Entity* newTarget) { this->setParent(newTarget); }

    glm::vec3 getOffset() { return transform.getPosition(); }
    void setOffset(glm::vec2 offset) { transform.setPosition(glm::vec3(offset.x, offset.y, 0.01f)); }
};

#endif //SP_POINTER_H
