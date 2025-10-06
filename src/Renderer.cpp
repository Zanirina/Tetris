#include "Renderer.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

#include "Shader.h"

Renderer::Renderer()
{
    shader = new Shader("shaders/pbr.vs", "shaders/pbr.fs");
    initCube();
}

Renderer::~Renderer()
{
    delete shader;
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
}

void Renderer::initCube()
{
    float vertices[] = {
        // positions          // normals
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
         1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,

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


void Renderer::drawCube(const glm::mat4 &model, const glm::vec3 &albedo, float metallic, float roughness, const glm::vec3 &camPos)
{
    shader->use();
    shader->setMat4("model", model);
    shader->setVec3("albedo", albedo);
    shader->setFloat("metallic", metallic);
    shader->setFloat("roughness", roughness);
    shader->setFloat("ao", 1.0f);
    shader->setVec3("camPos", camPos);
    // set lights (example)
    glm::vec3 lightPositions[4] = {
        glm::vec3(10.0f, 10.0f, 10.0f),
        glm::vec3(-10.0f, 10.0f, 10.0f),
        glm::vec3(10.0f, -10.0f, 10.0f),
        glm::vec3(-10.0f, -10.0f, 10.0f)
    };
    glm::vec3 lightColors[4] = {
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f)
    };
    for(int i=0;i<4;i++){
        shader->setVec3("lightPositions[" + std::to_string(i) + "]", lightPositions[i]);
        shader->setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);
    }

    glBindVertexArray(cubeVAO);
    // draw 36 vertices
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}
