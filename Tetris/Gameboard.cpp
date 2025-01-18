#include <iostream>
#include "Gameboard.h"
#include <iomanip>
#include "Point.h"
#include <cassert>

//Row = X-axis
//Col = Y-axis

Gameboard::Gameboard() {
	Gameboard::empty();
}

void Gameboard::fillRow( int indexAtY, int fillContent ) {
	//To validate the row
	if (indexAtY >= 0 && indexAtY < Gameboard::MAX_Y) {
		//Fill the specific row from left to right till the max_x with fillContent in para.
		for (int indexAtX = 0; indexAtX < Gameboard::MAX_X; ++indexAtX) {
			grid[indexAtY][indexAtX] = fillContent;
		}
	}
}

void Gameboard::empty() {
	//Goes from bottom to max_y 
	for (int indexAtY = 0; indexAtY < Gameboard::MAX_Y; indexAtY++) {
		//Goes from left to max_x
		for (int indexAtX = 0; indexAtX < Gameboard::MAX_X; indexAtX++) {
			grid[indexAtY][indexAtX] = Gameboard::EMPTY_BLOCK;
		}
	}
}

void Gameboard::printToConsole() const {

	//using namespace std;
	//Goes from bottom to max_y 
	for (int indexAtY = 0; indexAtY < Gameboard::MAX_Y; indexAtY++) {
		//Goes from left to max_x
		for (int indexAtX = 0; indexAtX < Gameboard::MAX_X; indexAtX++) {
			int printContent = grid[indexAtY][indexAtX];
			//When we have content at that index print content
			if (printContent != Gameboard::EMPTY_BLOCK) {
				std::cout << std::setw(2) << printContent;
			}
			else {
				std::cout << std::setw(2) << '.'; //else print "."
			}
		}
		std::cout << "\n";
	}
}

bool Gameboard::isValidPoint(const Point& point) const {
	// The point to be inside 0 to MAX
	int x = point.getX();
	int y = point.getY();
	return isValidPoint(x, y); 
}

bool Gameboard::isValidPoint(int x, int y) const {
	
	if (x >= 0 && x < Gameboard::MAX_X && y >= 0 && y < Gameboard::MAX_Y) {
		return true;
	}
	else {
		return false;
	}
}

int Gameboard::getContent(const Point& point) const {

	assert(isValidPoint(point));
	//Value on that index
	int contentOnGrid = grid[point.getY()][point.getX()];
	return contentOnGrid;
}

int Gameboard::getContent(int x, int y) const{

	assert(isValidPoint(x,y));
	int contentOnGrid = grid[y][x];
	return contentOnGrid;

}

void Gameboard::setContent(const Point& point, int content) {
	if (isValidPoint(point)) {
		grid[point.getY()][point.getX()] = content;
	}
}

void Gameboard::setContent(int x, int y, int content) {
	if (isValidPoint(x, y)) {
		grid[y][x] = content;
	}
}



void Gameboard::setContent(const std::vector<Point>& point, int content) {
	//loop through each item
	for (const auto& p : point) {
		if (isValidPoint(p)) { // Validate each point
			grid[p.getY()][p.getX()] = content;

		}
	}
}

bool Gameboard::isRowCompleted(int indexAtY) const {

	assert(isValidPoint(0, indexAtY));// Para1 is zero because we only have to test index at y in range
	for (int colNum = 0; colNum < Gameboard::MAX_X; colNum++) {
		if (Gameboard::getContent(colNum, indexAtY) == Gameboard::EMPTY_BLOCK) {
			return false;
		}
	}
	return true;
}

void Gameboard::copyRowIntoRow(int sourceRow, int targetRow) {
	//Copying from left to right
	for (int col = 0; col < Gameboard::MAX_X; col++) {
		// targetRow get assignment from sourceRow
		grid[targetRow][col] = grid[sourceRow][col];
	}
}

void Gameboard::removeRow(int indexAtY) {
	//Validate that point exists on y axis
	assert(isValidPoint(0, indexAtY));
	//Starts from that row goes uptill topmost row
	for (int i = indexAtY - 1; i >=  0 ; i--) {
		copyRowIntoRow(i, i + 1);
	}
	fillRow(0, Gameboard::EMPTY_BLOCK);
}

void Gameboard::removeRows(std::vector<int> removeRows) {
	//For each loop to iterate over each row
	for (int row : removeRows) {
		Gameboard::removeRow(row);
	}
}

std::vector<int> Gameboard::getCompletedRowIndices() const {
	std::vector<int> completedRows;
	for (int row = 0; row < Gameboard::MAX_Y; row++) {
		if (isRowCompleted(row) == true) {
			completedRows.push_back(row);
		}
	}
	return completedRows;
}

int Gameboard::removeCompletedRows() {
	std::vector<int> completedRows = getCompletedRowIndices();
	//As removeRows have a vector paramter it uses for each loop and remove all items
	removeRows(completedRows);
	int count = completedRows.size();
	return count;

}

bool Gameboard::areAllLocsEmpty(const std::vector<Point>& points) const {

	for (const Point& point : points) {
		//Validates point 
		if (isValidPoint(point) == true ) {
			//Check all point are -1
			if (grid[point.getY()][point.getX()] != Gameboard::EMPTY_BLOCK) {
				return false;
			}
		}
	}
	return true;
}

Point Gameboard::getSpawnLoc() {


	return spawnLoc;
}





