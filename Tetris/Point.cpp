#include "Point.h"
#include <sstream>

Point::Point() : x(0), y(0) {}
Point::Point(int x, int y) : x(x), y(y) {}

void Point::setXY(int x, int y) {
    this->x = x;
    this->y = y;
}

void Point::swapXY() {
    int temp = x;
    x = y;
    y = temp;
}

void Point::multiplyX(int factor) {
    x *= factor;
}

void Point::multiplyY(int factor) {
    y *= factor;
}

std::string Point::toString() const {
    std::ostringstream oss;
    oss << "[" << x << "," << y << "]";
    return oss.str();
}
