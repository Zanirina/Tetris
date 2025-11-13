// main.cpp
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
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
Renderer *rendererPtr = nullptr;
float lastTime = 0.0f;
float keyTimer = 0.0f;
const float KEY_COOLDOWN = 0.15f; // немного медленнее, чтобы не слишком чувствительно
float downKeyTimer = 0.0f;
const float DOWN_KEY_COOLDOWN = 0.03f;

glm::vec3 colors[] = {
    {0.2f, 0.8f, 0.8f}, // I
    {0.9f, 0.8f, 0.2f}, // O
    {0.7f, 0.2f, 0.8f}, // T
    {0.8f, 0.5f, 0.2f}, // L
    {0.2f, 0.4f, 0.8f}, // J
    {0.2f, 0.8f, 0.4f}, // S
    {0.8f, 0.2f, 0.2f}  // Z
};

void drawWalls(Renderer &renderer, const glm::vec3 &camPos) {
    glm::vec3 wallColor(0.4f, 0.4f, 0.5f);
    for (int y = -1; y < Game::HEIGHT + 1; ++y) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), {-0.8f, (float)y, 0.0f});
        model = glm::scale(model, {0.4f, 0.5f, 0.5f});
        renderer.drawCube(model, wallColor, 0.3f, 0.8f, camPos);

        model = glm::translate(glm::mat4(1.0f), {Game::WIDTH - 0.2f, (float)y, 0.0f});
        model = glm::scale(model, {0.4f, 0.5f, 0.5f});
        renderer.drawCube(model, wallColor, 0.3f, 0.8f, camPos);
    }

    for(int x=-1;x<Game::WIDTH+1;++x){
        glm::mat4 model = glm::translate(glm::mat4(1.0f), { (float)x, -0.8f, 0.0f});
        model = glm::scale(model, {0.5f, 0.4f, 0.5f});
        renderer.drawCube(model, wallColor, 0.3f, 0.8f, camPos);
    }

    for(int x=-2;x<Game::WIDTH+2;++x)
        for(int y=-2;y<Game::HEIGHT+2;++y){
            glm::mat4 model = glm::translate(glm::mat4(1.0f), {(float)x,(float)y,-0.6f});
            model = glm::scale(model,{0.5f,0.5f,0.4f});
            renderer.drawCube(model,{0.2f,0.2f,0.25f},0.4f,0.9f,camPos);
        }
}

void processInput(GLFWwindow *window, float dt, bool &wasGameOver) {
    keyTimer += dt;
    downKeyTimer += dt;
    static bool spacePressed = false;
    static bool rPressed = false;

    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        if(!spacePressed){
            game.hardDrop();
            spacePressed = true;
        }
    } else spacePressed = false;

    if(game.isGameOver() && glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
        if(!rPressed){
            game = Game();
            wasGameOver = false;
            rPressed = true;
        }
    } else rPressed = false;

    if(keyTimer >= KEY_COOLDOWN){
        if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){ game.moveLeft(); keyTimer=0;}
        if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){ game.moveRight(); keyTimer=0;}
        if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){ game.rotate(); keyTimer=0;}
    }

    if(downKeyTimer >= DOWN_KEY_COOLDOWN){
        if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){ game.moveDown(); downKeyTimer=0;}
    }
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height){
    windowWidth=width;
    windowHeight=height;
    glViewport(0,0,width,height);
    if(rendererPtr){
        Shader *sh = rendererPtr->getShader();
        sh->use();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),(float)width/height,0.1f,100.0f);
        sh->setMat4("projection",projection);
    }
}

int main(){
    if(!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(windowWidth,windowHeight,"Tetris PBR Prototype",nullptr,nullptr);
    if(!window){ glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;
    glEnable(GL_DEPTH_TEST);

    Renderer renderer;
    rendererPtr = &renderer;
    glm::vec3 camPos = {4.5f, 12.0f, 20.0f};
    Shader *sh = renderer.getShader();
    sh->use();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f),(float)windowWidth/windowHeight,0.1f,100.0f);
    sh->setMat4("projection",projection);
    glm::mat4 view = glm::lookAt(camPos,{4.5f,6.0f,0.0f},{0,1,0});
    sh->setMat4("view",view);

    lastTime = (float)glfwGetTime();

    // ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window,true);
    ImGui_ImplOpenGL3_Init("#version 330");

    bool wasGameOver = false;

    while(!glfwWindowShouldClose(window)){
        float time = (float)glfwGetTime();
        float dt = time - lastTime;
        lastTime = time;

        processInput(window, dt, wasGameOver);
        game.update(dt);

        glViewport(0,0,windowWidth,windowHeight);
        glClearColor(0.05f,0.05f,0.1f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawWalls(renderer, camPos);

        const auto &grid = game.getGrid();
        for(int y=0;y<Game::HEIGHT;++y)
            for(int x=0;x<Game::WIDTH;++x){
                int cellValue = grid[y*Game::WIDTH+x];
                if(cellValue!=0){
                    glm::mat4 model = glm::translate(glm::mat4(1.0f),{(float)x,(float)y,0.0f});
                    model = glm::scale(model,{0.45f,0.45f,0.45f});
                    renderer.drawCube(model, colors[cellValue-1],0.1f,0.7f,camPos);
                }
            }

        if(!game.isGameOver()){
            auto active = game.getActive();
            for(auto &c : active.cells){
                int gx = active.x + c.first;
                int gy = active.y + c.second;
                if(gx>=0 && gx<Game::WIDTH && gy>=0 && gy<Game::HEIGHT){
                    glm::mat4 model = glm::translate(glm::mat4(1.0f),{(float)gx,(float)gy,0.0f});
                    model = glm::scale(model,{0.45f,0.45f,0.45f});
                    renderer.drawCube(model,colors[active.colorIndex-1],0.3f,0.3f,camPos);
                }
            }
        }

        // ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos({10,10});
        ImGui::SetNextWindowSize({200,80});
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                                 ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground;
        if(ImGui::Begin("Score",nullptr,flags)){
            ImGui::Text("Score: %d", game.getScore());
            ImGui::Text("Lines: %d", game.getLines());
        }
        ImGui::End();

        if(game.isGameOver() && !wasGameOver) ImGui::OpenPopup("Game Over");
        wasGameOver = game.isGameOver();

        if(ImGui::BeginPopupModal("Game Over",nullptr,ImGuiWindowFlags_AlwaysAutoResize)){
            ImGui::Text("GAME OVER!");
            ImGui::Separator();
            ImGui::Text("Score: %d", game.getScore());
            ImGui::Text("Lines: %d", game.getLines());
            ImGui::Separator();
            ImGui::Text("Press R to restart");
            ImGui::EndPopup();
        }

        glDisable(GL_DEPTH_TEST);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glEnable(GL_DEPTH_TEST);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
