#include "GridTetromino.h"
#include "Tetromino.h"
#include <iostream>

void GridTetromino::move(int xOffset, int yOffset) {
    gridLoc.setX(xOffset + gridLoc.getX());
    gridLoc.setY(yOffset + gridLoc.getY());
}

std::vector<Point> GridTetromino::getBlockLocsMappedToGrid() const {
    std::vector<Point> mappedBlockLocs;

    // Call the method properly with parentheses to get blockLocs
    std::vector<Point> blockLocs = getBlockLocs();

    // Adjust each block's position by adding gridLoc's x and y to each block's coordinates
    for (const Point& block : blockLocs) {
        mappedBlockLocs.push_back(Point(block.getX() + gridLoc.getX(), block.getY() + gridLoc.getY()));
    }

    return mappedBlockLocs;  // Return the vector of mapped block locations
}
