//  Copyright (c) 2026 William Jackson
//  Licensed under the MIT License

//
// Created by Will on 3/31/2023.
//

#include "SPCursor.h"

#include "SpriteComponent2D.h"
#include "AnimationComponent.h"
#include "AssetManager.h"
#include "ColorAnimation.h"

SPCursor::SPCursor(Entity* target, glm::vec2 offset)
{
    setParent(target);
    transform.setPosition(glm::vec3(offset.x, offset.y, 0.01f));

    Shader shader = AssetManager::getInstance()->loadShader("shaders/sprite.vert",
                                                            "shaders/sprite.frag",
                                                            nullptr,
                                                            "sprite");
    Texture2D texture = AssetManager::getInstance()->loadTexture("assets/pointer.png", true, "pointer");

    SpriteComponent2D* spriteComponent;
    spriteComponent = new SpriteComponent2D(shader, texture, glm::vec2(30, 30));

    glm::vec4 origColor = spriteComponent->color;
    spriteComponent->setColor4(glm::vec4(origColor.x, origColor.y, origColor.z, 0.0f));
    addComponent(spriteComponent);

    auto* animationComponent = new AnimationComponent();
    addComponent(animationComponent);

    shouldUpdate = true;
}

void SPCursor::enable()
{
    if(enabled)
        return;

    enabled = true;

    auto* spriteComponent = getComponent<SpriteComponent2D>();
    auto* animationComponent = getComponent<AnimationComponent>();

    glm::vec4 origColor = spriteComponent->color;
    glm::vec4 targetColor = glm::vec4(origColor.x, origColor.y, origColor.z, 1.0f);

    animationComponent->skipAll();
    animationComponent->addAndStart<ColorAnimation>(&spriteComponent->color, targetColor, .1f);
}

void SPCursor::disable()
{
    if(!enabled)
        return;

    enabled = false;

    auto* spriteComponent = getComponent<SpriteComponent2D>();
    auto* animationComponent = getComponent<AnimationComponent>();

    glm::vec4 origColor = spriteComponent->color;
    glm::vec4 targetColor = glm::vec4(origColor.x, origColor.y, origColor.z, 0.0f);

    animationComponent->skipAll();
    animationComponent->addAndStart<ColorAnimation>(&spriteComponent->color, targetColor, .1f);
}