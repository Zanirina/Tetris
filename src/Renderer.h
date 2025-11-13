//Renderer.h
#pragma once
#include <glm/glm.hpp>
#include "Shader.h"

class Renderer {
public:
    Renderer();
    ~Renderer();
    void drawCube(const glm::mat4 &model, const glm::vec3 &albedo, float metallic, float roughness, const glm::vec3 &camPos);
    Shader* getShader() { return shader; }
private:
    unsigned int cubeVAO, cubeVBO;
    Shader* shader;
    void initCube();
};
