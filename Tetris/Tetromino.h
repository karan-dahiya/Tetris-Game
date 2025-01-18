#ifndef TETROMINO_H
#define TETROMINO_H
#include "Point.h"
#include <vector>

//Tetromino Color
enum TetColor {
	RED,
	ORANGE,
	YELLOW,
	GREEN,
	BLUE_LIGHT,
	BLUE_DARK,
	PURPLE
};

//Tetromino Shapes
enum TetShape {
	O, I, S, Z, L, J, T, PREV
};

class Tetromino {

	friend class TestSuite;
private:
	TetColor color;
	TetShape shape;
	std::vector<Point> blockLocs;

public:
	
	Tetromino() : color(TetColor::YELLOW), shape(TetShape::O) {
		setShape(shape);
	}

	TetColor getColor() const { return color; }

	TetShape getShape() const { return shape; }


	//Set shape for element
	void setShape(TetShape shape);
	
	// print a grid to display the current shape
	void printToConsole() const;
	
	// rotate the shape 90 degrees around [0,0] (clockwise)
	void rotateClockwise();

	/// Access to blockLocs
	std::vector<Point> getBlockLocs() const { return blockLocs; }

	static TetShape getRandomShape();
};

#endif