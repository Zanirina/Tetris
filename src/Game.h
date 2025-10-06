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
    void rotate(); // ADD THIS
    const std::vector<int>& getGrid() const { return grid; }
    Piece getActive() const { return active; }
    void spawnRandom();
    bool isGameOver() const { return gameOver; } // ADD THIS

private:
    std::vector<int> grid;
    Piece active;
    float fallTimer;
    float fallInterval;
    bool gameOver; // ADD THIS
    bool checkCollision(const Piece& p) const;
    void lockPiece();
    void clearLines();
};