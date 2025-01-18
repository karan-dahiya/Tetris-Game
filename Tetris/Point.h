#ifndef POINT_H
#define POINT_H
#include <iostream>

class Point {

	friend class TestSuite;
private: 
	int x;
	int y;

public:
	Point();

	Point(int x, int y);

	//Getter for X
	int getX() const { return x; }

	//Getter for Y
	int getY() const { return y; }

	//Setter for X
	void setX(int x) { this->x = x; }

	//Setter for Y
	void setY(int y) { this->y = y; }

	//Set X and Y
	void setXY(int x, int y);

	// swap x and y
	void swapXY();

	// multiply x by some factor
	void multiplyX(int factor);

	// multiply y by some factor
	void multiplyY(int factor);

	// return a string in the form "[x,y]" to represent the state of the Point instance
	// (for debugging) You could use stringstream if you want to try a forming a string in
	// the same way you use cout https://www.learncpp.com/cpp-tutorial/stream-classes-for-strings/
	std::string toString() const;
};

#endif