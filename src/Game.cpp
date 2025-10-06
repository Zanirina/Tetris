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
    int r = std::rand() % 4;
    Piece p;
    p.x = WIDTH / 2 - 1;
    p.y = HEIGHT - 4; // Start lower
    p.colorIndex = r + 1;

    if(r == 0) { // I
        p.cells = { std::make_pair(0,0), std::make_pair(0,1), std::make_pair(0,-1), std::make_pair(0,2) };
    } else if(r == 1) { // O
        p.cells = { std::make_pair(0,0), std::make_pair(1,0), std::make_pair(0,1), std::make_pair(1,1) };
    } else if(r == 2) { // L
        p.cells = { std::make_pair(0,0), std::make_pair(0,1), std::make_pair(0,-1), std::make_pair(1,-1) };
    } else { // T
        p.cells = { std::make_pair(0,0), std::make_pair(-1,0), std::make_pair(1,0), std::make_pair(0,1) };
    }

    active = p;

    // Check if game over
    if(checkCollision(active)) {
        gameOver = true;
        std::cout << "Game Over!" << std::endl;
    }
}

bool Game::checkCollision(const Piece& p) const
{
    for(auto &c : p.cells){
        int gx = p.x + c.first;
        int gy = p.y + c.second;
        if(gx < 0 || gx >= WIDTH || gy < 0) return true;
        if(gy < HEIGHT && grid[gy * WIDTH + gx] != 0) return true;
    }
    return false;
}

void Game::rotate()
{
    if(gameOver) return;

    Piece rotated = active;
    for(auto &c : rotated.cells) {
        int temp = c.first;
        c.first = -c.second;
        c.second = temp;
    }

    if(!checkCollision(rotated)) {
        active = rotated;
    }
}

void Game::lockPiece()
{
    for(auto &c : active.cells){
        int gx = active.x + c.first;
        int gy = active.y + c.second;
        if(gy >= 0 && gy < HEIGHT && gx >= 0 && gx < WIDTH)
            grid[gy * WIDTH + gx] = active.colorIndex;
    }
    clearLines();
    spawnRandom();
}

void Game::clearLines()
{
    for(int y = 0; y < HEIGHT; ++y){
        bool full = true;
        for(int x = 0; x < WIDTH; ++x){
            if(grid[y * WIDTH + x] == 0){ full = false; break; }
        }
        if(full){
            for(int yy = y; yy < HEIGHT-1; ++yy){
                for(int x = 0; x < WIDTH; ++x)
                    grid[yy * WIDTH + x] = grid[(yy+1) * WIDTH + x];
            }
            for(int x = 0; x < WIDTH; ++x)
                grid[(HEIGHT-1) * WIDTH + x] = 0;
            --y;
        }
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