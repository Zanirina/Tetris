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
const float DOWN_KEY_COOLDOWN = 0.05f; // Быстрее чем другие кнопки, но не мгновенно

// Color palette for different tetrominoes
glm::vec3 colors[] = {
    glm::vec3(0.8f, 0.2f, 0.2f), // Red - I
    glm::vec3(0.9f, 0.8f, 0.2f), // Yellow - O
    glm::vec3(0.2f, 0.6f, 0.9f), // Blue - L
    glm::vec3(0.7f, 0.2f, 0.8f)  // Purple - T
};

void processInput(GLFWwindow* window, float dt)
{
    keyTimer += dt;
    downKeyTimer += dt;

    // Cooldown for left/right/rotate to prevent too fast movement
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

    // Cooldown for down key - быстрее чем другие кнопки, но не мгновенно
    if(downKeyTimer >= DOWN_KEY_COOLDOWN) {
        if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            game.moveDown();
            downKeyTimer = 0.0f;
        }
    }

    // Instant action only for hard drop
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        game.hardDrop();
    }

    // Restart game when over
    if(game.isGameOver() && glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        // Для рестарта нужно пересоздать объект Game
        game = Game();
        std::cout << "Game Restarted!" << std::endl;
    }
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, width, height);

    // Update projection matrix
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

    // Camera setup - better view of the playing field
    glm::vec3 camPos = glm::vec3(5.0f, 8.0f, 15.0f);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)windowWidth/windowHeight, 0.1f, 100.0f);

    Shader* sh = renderer.getShader();
    sh->use();
    sh->setMat4("projection", projection);
    glm::mat4 view = glm::lookAt(camPos, glm::vec3(5.0f, 5.0f, 0.0f), glm::vec3(0,1,0));
    sh->setMat4("view", view);

    lastTime = (float)glfwGetTime();

    std::cout << "Tetris PBR Controls:" << std::endl;
    std::cout << "LEFT/RIGHT: Move piece" << std::endl;
    std::cout << "UP: Rotate piece" << std::endl;
    std::cout << "DOWN: Soft drop (faster fall)" << std::endl;
    std::cout << "SPACE: Hard drop (instant drop)" << std::endl;
    std::cout << "R: Restart game when game over" << std::endl;

    while(!glfwWindowShouldClose(window)){
        float time = (float)glfwGetTime();
        float dt = time - lastTime;
        lastTime = time;

        processInput(window, dt);

        // Update game logic
        game.update(dt);

        // Render
        glViewport(0, 0, windowWidth, windowHeight);
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw grid blocks (locked pieces)
        const auto& grid = game.getGrid();
        for(int y = 0; y < Game::HEIGHT; ++y){
            for(int x = 0; x < Game::WIDTH; ++x){
                int cellValue = grid[y * Game::WIDTH + x];
                if(cellValue != 0){
                    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x + 0.5f, y + 0.5f, 0.0f));
                    model = glm::scale(model, glm::vec3(0.9f)); // small gap between blocks

                    // Use color based on piece type
                    int colorIndex = cellValue - 1;
                    if(colorIndex >= 0 && colorIndex < 4) {
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

                // Only draw if within visible area
                if(gy >= 0 && gy < Game::HEIGHT && gx >= 0 && gx < Game::WIDTH) {
                    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(gx + 0.5f, gy + 0.5f, 0.0f));
                    model = glm::scale(model, glm::vec3(0.95f)); // Slightly larger than locked pieces

                    // Use color based on piece type
                    int colorIndex = active.colorIndex - 1;
                    if(colorIndex >= 0 && colorIndex < 4) {
                        glm::vec3 albedo = colors[colorIndex];
                        // Make active piece slightly metallic to distinguish it
                        renderer.drawCube(model, albedo, 0.3f, 0.3f, camPos);
                    }
                }
            }
        }

        // Draw game over message
        if(game.isGameOver()) {
            // In a real implementation, you'd render text here
            // For now, we'll just print to console
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