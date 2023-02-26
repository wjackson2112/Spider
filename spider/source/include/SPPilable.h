//
// Created by Will on 1/16/2023.
//

#ifndef SP_PILABLE_H
#define SP_PILABLE_H

#include "glm/glm.hpp"
#include "Entity.h"

#define STACK_MIN 0.0f
#define STACK_MAX 0.1f
#define STACK_OFFSET 0.0001f

class SPPilable : public Entity
{
protected:
    SPPilable* pileParent = nullptr;
    SPPilable* pileChild = nullptr;

    glm::vec3 rootOffset = glm::vec3(0.0f, 0.0f, STACK_OFFSET);
    glm::vec3 initialRootOffset = glm::vec3(0.0f, 0.0f, STACK_OFFSET);
    glm::vec3 pileOffset = glm::vec3(0.0f, 0.0f, STACK_OFFSET);
    glm::vec3 initialPileOffset = glm::vec3(0.0f, 0.0f, STACK_OFFSET);

public:
    glm::vec3 getRootOffset();
    glm::vec3 getPileOffset();

    glm::vec3 getInitialRootOffset();
    glm::vec3 getInitialPileOffset();

    void setRootOffset(glm::vec3 newOffset) { rootOffset = newOffset; };
    void setPileOffset(glm::vec3 newOffset) { pileOffset = newOffset; };

    void setInitialRootOffset(glm::vec3 newOffset) { initialRootOffset = newOffset; };
    void setInitialPileOffset(glm::vec3 newOffset) { initialPileOffset = newOffset; };

    SPPilable* getPileRoot();
    SPPilable* getPileEnd();
    SPPilable* getPileParent() { return pileParent; }
    SPPilable* getPileChild() { return pileChild; }

    void raiseToFront();
    void addToPile(SPPilable* pilable, bool snap = false);
    bool isInPile(SPPilable* pilable);
    void removeFromPile();
    virtual ~SPPilable() = default;
    virtual void moveTo(glm::vec3 target);
    virtual void snapTo(glm::vec3 target);

    virtual glm::vec2 getSize() { return glm::vec2(0,0); };
    virtual void setSize(glm::vec2 size) {};
};


#endif //SP_PILABLE_H
