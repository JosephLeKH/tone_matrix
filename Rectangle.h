#pragma once

#include <ostream>

struct Rectangle {
    int x, y, width, height;
};

std::ostream& operator<< (std::ostream& out, const Rectangle& rect);
bool operator== (const Rectangle& r1, const Rectangle& r2);
bool operator<  (const Rectangle& r1, const Rectangle& r2);
