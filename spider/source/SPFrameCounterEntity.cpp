//
// Created by Will on 1/21/2023.
//

#include "SPFrameCounterEntity.h"

#include "TextComponent.h"
#include "AssetManager.h"

SPFrameCounterEntity::SPFrameCounterEntity()
{
    std::string text = "0.0";
    Shader textShader = AssetManager::getInstance()->loadShader("shaders\\text.vert",
                                                                "shaders\\text.frag",
                                                                nullptr,
                                                                text);
    TextFont textFont = AssetManager::getInstance()->loadTextFont("assets\\arial.ttf",
                                                                  "arial12", 12);
    auto* textComponent = new TextComponent(textShader, textFont, text);

    textComponent->setColor(glm::vec3(.25f, .25f, .5f));
    textComponent->setTransform(glm::vec3(0.f, 0.f, 0.2f));
    addComponent(textComponent);

    receivesUpdates = true;
}

void SPFrameCounterEntity::update(float deltaTime)
{
    std::string numFrames = std::to_string((int) (1/deltaTime));
    getComponent<TextComponent>()->setText(numFrames);
}