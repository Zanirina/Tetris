#pragma once
#include <vector>
#include <array>

struct Piece {
    // хранит блоки в 4 координатах (x,y) относительнo origin
    std::array<std::pair<int,int>,4> cells;
    int x, y; // позиция origin в поле
    int colorIndex;
};

class Game {
public:
    static const int WIDTH = 10;
    static const int HEIGHT = 20;

    Game();
    void update(float dt); // dt в секундах
    void moveLeft();
    void moveRight();
    void moveDown(); // ускорение
    void hardDrop();
    const std::vector<int>& getGrid() const { return grid; }
    Piece getActive() const { return active; }
    void spawnRandom();

private:
    std::vector<int> grid; // WIDTH * HEIGHT, 0 = empty, >0 = color index
    Piece active;
    float fallTimer;
    float fallInterval;
    bool checkCollision(const Piece& p) const;
    void lockPiece();
    void clearLines();
};
