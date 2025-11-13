// Game.h
#pragma once
#include <vector>
#include <array>

struct Piece {
    std::array<std::pair<int,int>,4> cells;
    int x, y;
    int colorIndex;
};

class Game {
public:
    static const int WIDTH = 10;
    static const int HEIGHT = 20;

    Game();

    void update(float dt);
    void moveLeft();
    void moveRight();
    void moveDown();
    void hardDrop();
    void rotate();

    const std::vector<int>& getGrid() const { return grid; }
    Piece getActive() const { return active; }
    bool isGameOver() const { return gameOver; }
    int getScore() const { return score; }
    int getLines() const { return totalLines; }

private:
    std::vector<int> grid;
    Piece active;
    float fallTimer;
    float fallInterval;
    bool gameOver;
    int score = 0;
    int totalLines = 0;

    void spawnRandom();
    bool checkCollision(const Piece& p) const;
    void lockPiece();
    void clearLines();
};

