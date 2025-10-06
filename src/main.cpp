#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Renderer.h"
#include "Game.h"

int windowWidth = 1280;
int windowHeight = 720;

Game game;
Renderer* rendererPtr = nullptr;
float lastTime = 0.0f;

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) game.moveLeft();
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) game.moveRight();
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) game.moveDown();
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) game.hardDrop();
}

int main()
{
    if(!glfwInit()){
        std::cerr << "Failed to init GLFW\n"; return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Tetris PBR Prototype", nullptr, nullptr);
    if(!window){ std::cerr << "Failed to create window\n"; glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cerr << "Failed to init GLAD\n"; return -1;
    }
    glEnable(GL_DEPTH_TEST);

    Renderer renderer;
    rendererPtr = &renderer;

    // camera
    glm::vec3 camPos = glm::vec3(5.0f, 10.0f, 25.0f);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)windowWidth/windowHeight, 0.1f, 100.0f);

    Shader* sh = renderer.getShader();
    sh->use();
    sh->setMat4("projection", projection);
    glm::mat4 view = glm::lookAt(camPos, glm::vec3(5.0f, 10.0f, 0.0f), glm::vec3(0,1,0));
    sh->setMat4("view", view);

    lastTime = (float)glfwGetTime();
    while(!glfwWindowShouldClose(window)){
        float time = (float)glfwGetTime();
        float dt = time - lastTime;
        lastTime = time;

        processInput(window);
        game.update(dt);

        // render
        glViewport(0,0,windowWidth,windowHeight);
        glClearColor(0.1f,0.1f,0.15f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw grid blocks
        const auto& grid = game.getGrid();
        for(int y=0;y<Game::HEIGHT;++y){
            for(int x=0;x<Game::WIDTH;++x){
                int v = grid[y * Game::WIDTH + x];
                if(v != 0){
                    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x + 0.5f, y + 0.5f, 0.0f));
                    model = glm::scale(model, glm::vec3(0.9f)); // small gap
                    glm::vec3 al(0.7f, 0.2f, 0.2f);
                    renderer.drawCube(model, al, 0.1f, 0.7f, camPos);
                }
            }
        }

        // draw active piece
        auto active = game.getActive();
        for(auto &c : active.cells){
            int gx = active.x + c.first;
            int gy = active.y + c.second;
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(gx + 0.5f, gy + 0.5f, 0.0f));
            model = glm::scale(model, glm::vec3(0.95f));
            glm::vec3 al(0.2f, 0.6f, 0.9f);
            renderer.drawCube(model, al, 0.0f, 0.5f, camPos);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
