//
// Created by Will on 3/31/2023.
//

#include "SPCursor.h"

#include "SpriteComponent2D.h"
#include "AnimationComponent.h"
#include "AssetManager.h"

SPCursor::SPCursor(Entity* target, glm::vec2 offset)
{
    setParent(target);
    transform.setPosition(glm::vec3(offset.x, offset.y, 0.01f));

    Shader shader = AssetManager::getInstance()->loadShader("shaders\\sprite.vert",
                                                            "shaders\\sprite.frag",
                                                            nullptr,
                                                            "sprite");
    Texture2D texture = AssetManager::getInstance()->loadTexture("assets\\pointer.png", true, "pointer");

    SpriteComponent2D* spriteComponent;
    spriteComponent = new SpriteComponent2D(shader, texture, glm::vec2(30, 30));
    addComponent(spriteComponent);

//    auto* animationComponent = new AnimationComponent();
//    addComponent(animationComponent);
}