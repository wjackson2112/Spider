//
// Created by Will on 1/9/2023.
//

#include "SPCard.h"

#include "AssetManager.h"
#include "EntityManager.h"
#include "SpriteSheetComponent2D.h"

SPCard::SPCard()
{
    this->transform.setPosition(glm::vec3(10, 10, 0.0f));

    Shader shader = AssetManager::getInstance()->loadShader("shaders\\sprite.vert",
                                                            "shaders\\sprite.frag",
                                                            nullptr,
                                                            "sprite");
    // Deck asset is from here: https://www.codeproject.com/Articles/1187548/Video-Poker
    Texture2D texture = AssetManager::getInstance()->loadTexture("assets\\deck.png", true, "deck");

    auto* spriteComponent = new SpriteSheetComponent2D(shader, texture, glm::vec2(49.f, 64.f), glm::vec2(13, 4), SPVALUE_EIGHT, SPSUIT_HEART);
    addComponent(spriteComponent);
}