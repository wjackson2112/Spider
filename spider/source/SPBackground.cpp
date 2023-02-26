//
// Created by Will on 6/24/2022.
//

#include "SPBackground.h"
#include "AssetManager.h"
#include "Texture2D.h"
#include "SpriteComponent2D.h"
#include "OptionsManager.h"

SPBackground::SPBackground()
{
    this->transform.setPosition(glm::vec3(this->transform.getPosition().x,
                                          this->transform.getPosition().y,
                                          -0.3f));
    Shader shader = AssetManager::getInstance()->loadShader("shaders\\sprite.vert",
                                                            "shaders\\sprite.frag",
                                                            nullptr,
                                                            "sprite");
    Texture2D texture = AssetManager::getInstance()->loadTexture("assets\\green_felt.png", true, "green_felt");
    glm::vec2 viewportRes = OptionsManager::getInstance()->getViewportResolution();
    auto* spriteComponent = new SpriteComponent2D(shader, texture, viewportRes);
    addComponent(spriteComponent);

    OptionsManager::getInstance()->registerReceiver(this);
}

SPBackground::~SPBackground()
{
    OptionsManager::getInstance()->deregisterReceiver(this);
}

void SPBackground::resolutionUpdated(glm::vec2 oldRes, glm::vec2 newRes)
{
    getComponent<SpriteComponent2D>()->setSize(newRes);
}