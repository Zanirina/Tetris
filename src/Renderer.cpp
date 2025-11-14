#include "Renderer.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include "Shader.h"

Renderer::Renderer() {
    shader = new Shader("shaders/pbr.vs", "shaders/pbr.fs");
    initCube();
}

Renderer::~Renderer() {
    delete shader;
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
}

void Renderer::initCube() {
    float vertices[] = {
        // positions          // normals
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
         1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,

        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
        -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
        -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,

         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
         1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
         1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,

        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
         1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
        -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,

        -1.0f,  1.0f, -1.0f,  0.0f, 1.0f,  0.0f,
         1.0f,  1.0f, -1.0f,  0.0f, 1.0f,  0.0f,
         1.0f,  1.0f,  1.0f,  0.0f, 1.0f,  0.0f,
         1.0f,  1.0f,  1.0f,  0.0f, 1.0f,  0.0f,
        -1.0f,  1.0f,  1.0f,  0.0f, 1.0f,  0.0f,
        -1.0f,  1.0f, -1.0f,  0.0f, 1.0f,  0.0f
    };

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
}

void Renderer::drawCube(const glm::mat4 &model, const glm::vec3 &albedo,
                        float metallic, float roughness, const glm::vec3 &camPos)
{
    shader->use();
    shader->setMat4("model", model);

    shader->setVec3("objectColor", albedo);
    shader->setVec3("camPos", camPos);

    shader->setFloat("metallic", metallic);
    shader->setFloat("roughness", roughness);

    // Glow
    shader->setFloat("emissionStrength", 0.3f);
    shader->setVec3("emissionColor", glm::vec3(1.0, 0.9, 0.8));

    // Fade-in (будет управляться из Game)
    shader->setFloat("fade", currentFadeValue);//currentFadeValue is red

    // Fog
    shader->setInt("useFog", 1);
    shader->setVec3("fogColor", glm::vec3(0.1f, 0.3f, 0.45f));
    shader->setFloat("fogNear", 15.0f);
    shader->setFloat("fogFar", 45.0f);

    // Lights (осветлил!)
    glm::vec3 lightPositions[2] = {
        glm::vec3(10, 10, 10),
        glm::vec3(-10, 10, 5)
    };
    glm::vec3 lightColors[2] = {
        glm::vec3(20.0f),
        glm::vec3(12.0f, 10.0f, 8.0f)
    };

    shader->setVec3("lightPositions[0]", lightPositions[0]);
    shader->setVec3("lightColors[0]", lightColors[0]);

    shader->setVec3("lightPositions[1]", lightPositions[1]);
    shader->setVec3("lightColors[1]", lightColors[1]);

    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}
