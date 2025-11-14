#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iostream>

Game::Game()
    : grid(WIDTH * HEIGHT, 0),
      fallTimer(0.0f),
      fallInterval(0.7f),
      gameOver(false),
      score(0),
      totalLines(0),
      fadeTimer(0.0f),//is red
      fadeValue(0.0f)//is red
{
    std::srand((unsigned)std::time(nullptr));
    spawnRandom();
}

// -----------------------------------------------------
// SPAWN NEW PIECE (with fade-in reset)
// -----------------------------------------------------
void Game::spawnRandom()
{
    int r = std::rand() % 7;

    Piece p;
    p.x = WIDTH / 2 - 2;
    p.y = HEIGHT - 1;
    p.colorIndex = r + 1;

    // Fade-in reset
    fadeTimer = 0.0f;//is red
    fadeValue = 0.0f;//is red

    switch(r) {
        case 0: p.cells = {{ {0,0},{1,0},{2,0},{3,0} }}; break;               // I
        case 1: p.cells = {{ {0,0},{1,0},{0,1},{1,1} }}; break;               // O
        case 2: p.cells = {{ {1,0},{0,1},{1,1},{2,1} }}; break;               // T
        case 3: p.cells = {{ {0,0},{0,1},{0,2},{1,2} }}; break;               // L
        case 4: p.cells = {{ {1,0},{1,1},{1,2},{0,2} }}; break;               // J
        case 5: p.cells = {{ {1,0},{2,0},{0,1},{1,1} }}; break;               // S
        case 6: p.cells = {{ {0,0},{1,0},{1,1},{2,1} }}; break;               // Z
    }

    active = p;

    if(checkCollision(active)) {
        gameOver = true;
        std::cout << "Game Over! Cannot spawn new piece." << std::endl;
    }
}

// -----------------------------------------------------
// COLLISION CHECK
// -----------------------------------------------------
bool Game::checkCollision(const Piece& p) const
{
    for(auto &c : p.cells){
        int gx = p.x + c.first;
        int gy = p.y + c.second;

        if(gx < 0 || gx >= WIDTH || gy < 0) return true;

        if(gy < HEIGHT){
            if(grid[gy * WIDTH + gx] != 0) return true;
        }
    }
    return false;
}

// -----------------------------------------------------
// ROTATION WITH WALL KICKS
// -----------------------------------------------------
void Game::rotate()
{
    if(gameOver) return;

    Piece rotated = active;
    for(auto &c : rotated.cells){
        int t = c.first;
        c.first = -c.second;
        c.second = t;
    }

    if(!checkCollision(rotated)){
        active = rotated;
        return;
    }

    // wall kicks
    Piece k = rotated;
    k.x = active.x - 1;
    if(!checkCollision(k)){ active = k; return; }

    k.x = active.x + 1;
    if(!checkCollision(k)){ active = k; return; }

    k.x = active.x;
    k.y = active.y + 1;
    if(!checkCollision(k)){ active = k; return; }
}

// -----------------------------------------------------
// LOCK PIECE INTO GRID
// -----------------------------------------------------
void Game::lockPiece()
{
    for(auto &c : active.cells){
        int gx = active.x + c.first;
        int gy = active.y + c.second;

        if(gy >= 0 && gy < HEIGHT && gx >= 0 && gx < WIDTH){
            grid[gy * WIDTH + gx] = active.colorIndex;
        }
    }

    score += 10;

    clearLines();
    spawnRandom();
}

// -----------------------------------------------------
// CLEAR FULL ROWS
// -----------------------------------------------------
void Game::clearLines()
{
    int cleared = 0;

    for(int y = 0; y < HEIGHT; y++){
        bool full = true;

        for(int x = 0; x < WIDTH; x++){
            if(grid[y * WIDTH + x] == 0){
                full = false;
                break;
            }
        }

        if(full){
            cleared++;

            for(int yy = y; yy < HEIGHT - 1; yy++){
                for(int x = 0; x < WIDTH; x++)
                    grid[yy * WIDTH + x] = grid[(yy+1) * WIDTH + x];
            }

            for(int x = 0; x < WIDTH; x++)
                grid[(HEIGHT-1) * WIDTH + x] = 0;

            y--;
        }
    }

    if(cleared > 0){
        totalLines += cleared;
        score += cleared * 100;
    }
}

// -----------------------------------------------------
// UPDATE LOGIC (gravity + fade animation)
// -----------------------------------------------------
void Game::update(float dt)
{
    if(gameOver) return;

    // Fade animation
    fadeTimer += dt;//is red
    fadeValue = std::min(1.0f, fadeTimer * 3.0f); //is red

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

// -----------------------------------------------------
// MOVEMENT
// -----------------------------------------------------
void Game::moveLeft()
{
    if(gameOver) return;
    Piece m = active; m.x -= 1;

    if(!checkCollision(m)) active = m;
}

void Game::moveRight()
{
    if(gameOver) return;
    Piece m = active; m.x += 1;

    if(!checkCollision(m)) active = m;
}

void Game::moveDown()
{
    if(gameOver) return;

    Piece m = active;
    m.y -= 1;

    if(!checkCollision(m)) active = m;
    else lockPiece();
}

// -----------------------------------------------------
// HARD DROP
// -----------------------------------------------------
void Game::hardDrop()
{
    if(gameOver) return;

    Piece m = active;

    while(!checkCollision(m))
        m.y -= 1;

    m.y += 1;

    active = m;
    lockPiece();
}
