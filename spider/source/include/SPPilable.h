//
// Created by Will on 1/16/2023.
//

#ifndef SP_PILABLE_H
#define SP_PILABLE_H

#include "glm/glm.hpp"
#include "Entity.h"

#define STACK_OFFSET 0.0001f

class SPPilable : public Entity
{
protected:
    SPPilable* pileParent = nullptr;
    SPPilable* pileChild = nullptr;

    glm::vec3 rootOffset = glm::vec3(0.0f, 0.0f, STACK_OFFSET);
    glm::vec3 pileOffset = glm::vec3(0.0f, 0.0f, STACK_OFFSET);

public:
    glm::vec3 getRootOffset();
    glm::vec3 getPileOffset();

    SPPilable* getPileRoot();
    SPPilable* getPileEnd();
    SPPilable* getPileParent() { return pileParent; }
    SPPilable* getPileChild() { return pileChild; }

    void addToPile(SPPilable* pilable);
    bool isInPile(SPPilable* pilable);
    void removeFromPile();
    virtual ~SPPilable() = default;
    virtual glm::vec2 getSize() { return glm::vec2(0,0); };
};


#endif //SP_PILABLE_H
