//
// Created by Will on 1/16/2023.
//

#include "SPPile.h"

#include <iostream>

#include "AssetManager.h"
#include "SpriteComponent2D.h"
#include "CollisionComponent2DAABB.h"

SPPile::SPPile(glm::vec2 position, glm::vec3 inRootOffset, glm::vec3 inPileOffset, bool incrementing, SPSnapValidator* validator)
: size(49.f, 64.f)
, incrementing(incrementing)
, validator(validator)
{
    rootOffset = inRootOffset;
    pileOffset = inPileOffset;

    initialRootOffset = inRootOffset;
    initialPileOffset = inPileOffset;

    this->transform.setPosition2(position);

    Shader shader = AssetManager::getInstance()->loadShader("shaders/sprite.vert",
                                                            "shaders/sprite.frag",
                                                            nullptr,
                                                            "sprite");
    // Deck asset is from here: https://www.codeproject.com/Articles/1187548/Video-Poker
    Texture2D texture = AssetManager::getInstance()->loadTexture("assets/pile_border.png", true, "pile_border");

    auto* spriteComponent = new SpriteComponent2D(shader, texture, size);
    addComponent(spriteComponent);

    shouldUpdate = true;
}

SPPile::~SPPile()
{
    removeFromPile();
    if(pileChild)
        pileChild->removeFromPile();
}

void SPPile::setSize(glm::vec2 size)
{
    this->size = size;
    getComponent<SpriteComponent2D>()->setSize(size);
}