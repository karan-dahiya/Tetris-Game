#include <iostream>
#include "Tetromino.h"

void Tetromino::setShape(TetShape nShape) {
    shape = nShape;
    blockLocs.clear();

    switch (shape) {
    case TetShape::O:   color = TetColor::YELLOW;       blockLocs = { Point(0, 0), Point(1, 0), Point(0, 1), Point(1, 1) };     break;
    case TetShape::I:   color = TetColor::BLUE_LIGHT;   blockLocs = { Point(0, 0), Point(0, 1), Point(0, 2), Point(0, -1) };    break;
    case TetShape::S:   color = TetColor::RED;          blockLocs = { Point(0, 0), Point(1, 0), Point(0, 1), Point(-1, 1) };    break;
    case TetShape::Z:   color = TetColor::GREEN;        blockLocs = { Point(0, 0), Point(-1, 0), Point(0, 1), Point(1, 1) };    break;
    case TetShape::L:   color = TetColor::ORANGE;       blockLocs = { Point(0, 0), Point(0, 1), Point(1, 1), Point(0, -1) };    break;
    case TetShape::J:   color = TetColor::BLUE_DARK;    blockLocs = { Point(0, 0), Point(0, 1), Point(-1, 1), Point(0, -1) };   break;
    case TetShape::T:   color = TetColor::PURPLE;       blockLocs = { Point(0, 0), Point(1, 0), Point(0, 1), Point(-1, 0) };    break;
    }
}

void Tetromino::rotateClockwise() {

    for (auto& point : blockLocs) {
        /*int x = point.getX();
        int y = point.getY();*/

        point.multiplyY(-1);
        point.swapXY();
    }
}

void Tetromino::printToConsole() const {
    const int SIZE{ 6 }; // grid's size
    char grid[SIZE][SIZE];

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            grid[i][j] = '.'; 
        }
    }

    for (const auto& point : blockLocs) {
        int x = point.getX() + 3; 
        int y = point.getY() + 3; 
        if (x >= 0 && x < SIZE && y >= 0 && y < SIZE) {
            grid[y][x] = 'X'; 
        }
    }

    // Print grid
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            std::cout << grid[i][j];
        }
        std::cout << std::endl;
    }
}

TetShape Tetromino::getRandomShape() {

    int random = rand() % static_cast<int>(TetShape::PREV);

    return static_cast<TetShape>(random);
}

