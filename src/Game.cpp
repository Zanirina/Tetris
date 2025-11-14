//Game.cpp
#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iostream>

Game::Game() : grid(WIDTH * HEIGHT, 0), fallTimer(0.0f), fallInterval(0.7f), gameOver(false)
{
    std::srand((unsigned)std::time(nullptr));
    spawnRandom();
}

void Game::spawnRandom()
{
    int r = std::rand() % 7; // 7 фигур вместо 4
    Piece p;
    p.x = WIDTH / 2 - 2; // Центрирование
    p.y = HEIGHT - 1;    // Появление сверху
    p.colorIndex = r + 1;

    // Правильные тетрамино (как в настоящем Тетрисе)
    switch(r) {
        case 0: // I
            p.cells = {
            std::make_pair(0,0), std::make_pair(1,0),
            std::make_pair(2,0), std::make_pair(3,0)
        };
            break;
        case 1: // O
            p.cells = {
            std::make_pair(0,0), std::make_pair(1,0),
            std::make_pair(0,1), std::make_pair(1,1)
        };
            break;
        case 2: // T
            p.cells = {
            std::make_pair(1,0), std::make_pair(0,1),
            std::make_pair(1,1), std::make_pair(2,1)
        };
            break;
        case 3: // L
            p.cells = {
            std::make_pair(0,0), std::make_pair(0,1),
            std::make_pair(0,2), std::make_pair(1,2)
        };
            break;
        case 4: // J
            p.cells = {
            std::make_pair(1,0), std::make_pair(1,1),
            std::make_pair(1,2), std::make_pair(0,2)
        };
            break;
        case 5: // S
            p.cells = {
            std::make_pair(1,0), std::make_pair(2,0),
            std::make_pair(0,1), std::make_pair(1,1)
        };
            break;
        case 6: // Z
            p.cells = {
            std::make_pair(0,0), std::make_pair(1,0),
            std::make_pair(1,1), std::make_pair(2,1)
        };
            break;
    }

    active = p;

    // Проверка Game Over
    if(checkCollision(active)) {
        gameOver = true;
        std::cout << "Game Over! Cannot spawn new piece." << std::endl;
    }
}

bool Game::checkCollision(const Piece& p) const
{
    for(auto &c : p.cells){
        int gx = p.x + c.first;
        int gy = p.y + c.second;

        // Проверка границ (стенок)
        if(gx < 0 || gx >= WIDTH || gy < 0) return true;

        // Проверка столкновения с заблокированными фигурами
        if(gy < HEIGHT) {
            if(grid[gy * WIDTH + gx] != 0) return true;
        }
    }
    return false;
}

void Game::rotate()
{
    if(gameOver) return;

    Piece rotated = active;

    // Матрица вращения 90° по часовой стрелке
    for(auto &c : rotated.cells) {
        int temp = c.first;
        c.first = -c.second;
        c.second = temp;
    }

    // Попробуем вращение, если не получается - откат
    if(!checkCollision(rotated)) {
        active = rotated;
        return;
    }

    // Wall kicks - попробуем сдвинуть при вращении
    Piece kicked = rotated;

    // Попробуем сдвинуть влево
    kicked.x = active.x - 1;
    if(!checkCollision(kicked)) {
        active = kicked;
        return;
    }

    // Попробуем сдвинуть вправо
    kicked.x = active.x + 1;
    if(!checkCollision(kicked)) {
        active = kicked;
        return;
    }

    // Попробуем сдвинуть вверх
    kicked.x = active.x;
    kicked.y = active.y + 1;
    if(!checkCollision(kicked)) {
        active = kicked;
        return;
    }
}

void Game::lockPiece()
{
    for(auto &c : active.cells){
        int gx = active.x + c.first;
        int gy = active.y + c.second;

        // Проверяем, не выходит ли за пределы
        if(gy >= 0 && gy < HEIGHT && gx >= 0 && gx < WIDTH) {
            grid[gy * WIDTH + gx] = active.colorIndex;
        }
    }
    clearLines();
    spawnRandom();
}
// Updated parts of Game.cpp (replace/add these functions)

// In clearLines():
void Game::clearLines()
{
    int linesCleared = 0;
    for(int y = 0; y < HEIGHT; ++y){
        bool full = true;
        for(int x = 0; x < WIDTH; ++x){
            if(grid[y * WIDTH + x] == 0){ full = false; break; }
        }
        if(full){
            linesCleared++;
            for(int yy = y; yy < HEIGHT-1; ++yy){
                for(int x = 0; x < WIDTH; ++x)
                    grid[yy * WIDTH + x] = grid[(yy+1) * WIDTH + x];
            }
            for(int x = 0; x < WIDTH; ++x)
                grid[(HEIGHT-1) * WIDTH + x] = 0;
            --y;  // Re-check the same row after shifting
        }
    }
    if(linesCleared > 0) {
        totalLines += linesCleared;
        score += linesCleared * 100;  // Simple scoring: 100 per line
    }
}

void Game::update(float dt)
{
    if(gameOver) return;

    fallTimer += dt;
    if(fallTimer >= fallInterval){
        fallTimer = 0.0f;
        Piece moved = active;
        moved.y -= 1;
        if(!checkCollision(moved)){
            active = moved;
        } else {
            lockPiece();
        }
    }
}

void Game::moveLeft()
{
    if(gameOver) return;
    Piece moved = active; moved.x -= 1;
    if(!checkCollision(moved)) active = moved;
}

void Game::moveRight()
{
    if(gameOver) return;
    Piece moved = active; moved.x += 1;
    if(!checkCollision(moved)) active = moved;
}

void Game::moveDown()
{
    if(gameOver) return;
    Piece moved = active; moved.y -= 1;
    if(!checkCollision(moved)) active = moved;
    else lockPiece();
}

void Game::hardDrop()
{
    if(gameOver) return;
    Piece moved = active;
    while(!checkCollision(moved)){
        moved.y -= 1;
    }
    moved.y += 1; // Go back to last valid position
    active = moved;
    lockPiece();
}