#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

Game::Game() : grid(WIDTH * HEIGHT, 0), fallTimer(0.0f), fallInterval(0.7f)
{
    std::srand((unsigned)std::time(nullptr));
    spawnRandom();
}

void Game::spawnRandom()
{
    // несколько простых фигур (I, O, L, T)
    int r = std::rand() % 4;
    Piece p;
    p.x = WIDTH / 2 - 1;
    p.y = HEIGHT - 2;
    p.colorIndex = (r+1);
    if(r == 0) { // I (vertical)
        p.cells = { std::make_pair(0,0), std::make_pair(0,1), std::make_pair(0,-1), std::make_pair(0,2) };
    } else if(r == 1) { // O
        p.cells = { std::make_pair(0,0), std::make_pair(1,0), std::make_pair(0,1), std::make_pair(1,1) };
    } else if(r == 2) { // L
        p.cells = { std::make_pair(0,0), std::make_pair(0,1), std::make_pair(0,-1), std::make_pair(1,-1) };
    } else { // T
        p.cells = { std::make_pair(0,0), std::make_pair(-1,0), std::make_pair(1,0), std::make_pair(0,1) };
    }
    active = p;
}

bool Game::checkCollision(const Piece& p) const
{
    for(auto &c : p.cells){
        int gx = p.x + c.first;
        int gy = p.y + c.second;
        if(gx < 0 || gx >= WIDTH || gy < 0) return true;
        if(gy < HEIGHT) {
            if(grid[gy * WIDTH + gx] != 0) return true;
        }
    }
    return false;
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
            // remove line
            for(int yy = y; yy < HEIGHT-1; ++yy){
                for(int x = 0; x < WIDTH; ++x)
                    grid[yy * WIDTH + x] = grid[(yy+1) * WIDTH + x];
            }
            // clear top
            for(int x = 0; x < WIDTH; ++x) grid[(HEIGHT-1) * WIDTH + x] = 0;
            --y; // recheck same y index
        }
    }
}

void Game::update(float dt)
{
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
    Piece moved = active; moved.x -= 1;
    if(!checkCollision(moved)) active = moved;
}
void Game::moveRight()
{
    Piece moved = active; moved.x += 1;
    if(!checkCollision(moved)) active = moved;
}
void Game::moveDown()
{
    Piece moved = active; moved.y -= 1;
    if(!checkCollision(moved)) active = moved;
    else { lockPiece(); }
}
void Game::hardDrop()
{
    Piece moved = active;
    while(true){
        moved.y -= 1;
        if(checkCollision(moved)) { moved.y += 1; break; }
    }
    active = moved;
    lockPiece();
}
