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
float keyTimer = 0.0f;
const float KEY_COOLDOWN = 0.1f;
float downKeyTimer = 0.0f;
const float DOWN_KEY_COOLDOWN = 0.05f;

// Color palette for all 7 tetrominoes
glm::vec3 colors[] = {
    glm::vec3(0.2f, 0.8f, 0.8f), // Cyan - I
    glm::vec3(0.9f, 0.8f, 0.2f), // Yellow - O
    glm::vec3(0.7f, 0.2f, 0.8f), // Purple - T
    glm::vec3(0.8f, 0.5f, 0.2f), // Orange - L
    glm::vec3(0.2f, 0.4f, 0.8f), // Blue - J
    glm::vec3(0.2f, 0.8f, 0.4f), // Green - S
    glm::vec3(0.8f, 0.2f, 0.2f)  // Red - Z
};

void drawWalls(Renderer& renderer, const glm::vec3& camPos)
{
    // Цвет стен
    glm::vec3 wallColor(0.3f, 0.3f, 0.4f);

    // Левая стенка
    for(int y = 0; y < Game::HEIGHT + 1; ++y) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, y, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        renderer.drawCube(model, wallColor, 0.3f, 0.8f, camPos);
    }

    // Правая стенка
    for(int y = 0; y < Game::HEIGHT + 1; ++y) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(Game::WIDTH, y, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        renderer.drawCube(model, wallColor, 0.3f, 0.8f, camPos);
    }

    // Нижняя стенка (пол)
    for(int x = -1; x < Game::WIDTH + 1; ++x) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, -0.5f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        renderer.drawCube(model, wallColor, 0.3f, 0.8f, camPos);
    }

    // Фон (задняя стенка) - опционально
    for(int x = -1; x < Game::WIDTH + 1; ++x) {
        for(int y = -1; y < Game::HEIGHT + 1; ++y) {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, -0.5f));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            renderer.drawCube(model, glm::vec3(0.2f, 0.2f, 0.25f), 0.4f, 0.9f, camPos);
        }
    }
}

void processInput(GLFWwindow* window, float dt)
{
    keyTimer += dt;
    downKeyTimer += dt;

    if(keyTimer >= KEY_COOLDOWN) {
        if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            game.moveLeft();
            keyTimer = 0.0f;
        }
        if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            game.moveRight();
            keyTimer = 0.0f;
        }
        if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            game.rotate();
            keyTimer = 0.0f;
        }
    }

    if(downKeyTimer >= DOWN_KEY_COOLDOWN) {
        if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            game.moveDown();
            downKeyTimer = 0.0f;
        }
    }

    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        game.hardDrop();
    }

    if(game.isGameOver() && glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        game = Game();
        std::cout << "Game Restarted!" << std::endl;
    }
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, width, height);

    if (rendererPtr) {
        Shader* sh = rendererPtr->getShader();
        sh->use();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width/height, 0.1f, 100.0f);
        sh->setMat4("projection", projection);
    }
}

int main()
{
    if(!glfwInit()){
        std::cerr << "Failed to init GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Tetris PBR Prototype", nullptr, nullptr);
    if(!window){
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cerr << "Failed to init GLAD\n";
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Renderer renderer;
    rendererPtr = &renderer;

    // Better camera view
    glm::vec3 camPos = glm::vec3(4.5f, 12.0f, 20.0f);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)windowWidth/windowHeight, 0.1f, 100.0f);

    Shader* sh = renderer.getShader();
    sh->use();
    sh->setMat4("projection", projection);
    glm::mat4 view = glm::lookAt(camPos, glm::vec3(4.5f, 6.0f, 0.0f), glm::vec3(0,1,0));
    sh->setMat4("view", view);

    lastTime = (float)glfwGetTime();

    std::cout << "Tetris PBR Controls:" << std::endl;
    std::cout << "LEFT/RIGHT: Move piece" << std::endl;
    std::cout << "UP: Rotate piece" << std::endl;
    std::cout << "DOWN: Soft drop" << std::endl;
    std::cout << "SPACE: Hard drop" << std::endl;
    std::cout << "R: Restart game" << std::endl;

    while(!glfwWindowShouldClose(window)){
        float time = (float)glfwGetTime();
        float dt = time - lastTime;
        lastTime = time;

        processInput(window, dt);
        game.update(dt);

        glViewport(0, 0, windowWidth, windowHeight);
        glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw walls first
        drawWalls(renderer, camPos);

        // Draw grid blocks (locked pieces)
        const auto& grid = game.getGrid();
        for(int y = 0; y < Game::HEIGHT; ++y){
            for(int x = 0; x < Game::WIDTH; ++x){
                int cellValue = grid[y * Game::WIDTH + x];
                if(cellValue != 0){
                    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f));
                    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

                    int colorIndex = cellValue - 1;
                    if(colorIndex >= 0 && colorIndex < 7) {
                        glm::vec3 albedo = colors[colorIndex];
                        renderer.drawCube(model, albedo, 0.1f, 0.7f, camPos);
                    }
                }
            }
        }

        // Draw active piece
        if(!game.isGameOver()) {
            auto active = game.getActive();
            for(auto &c : active.cells){
                int gx = active.x + c.first;
                int gy = active.y + c.second;

                if(gy >= 0 && gy < Game::HEIGHT && gx >= 0 && gx < Game::WIDTH) {
                    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(gx, gy, 0.0f));
                    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

                    int colorIndex = active.colorIndex - 1;
                    if(colorIndex >= 0 && colorIndex < 7) {
                        glm::vec3 albedo = colors[colorIndex];
                        renderer.drawCube(model, albedo, 0.3f, 0.3f, camPos);
                    }
                }
            }
        }

        if(game.isGameOver()) {
            static bool gameOverPrinted = false;
            if(!gameOverPrinted) {
                std::cout << "GAME OVER! Press R to restart" << std::endl;
                gameOverPrinted = true;
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}