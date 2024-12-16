#include "Rectangle.h"
#include "GUI/TextUtils.h"
#include "collections.h"
using namespace std;

ostream& operator<< (ostream& out, const Rectangle& rect) {
    return out << format("{ %s, %s, %s, %s }", rect.x, rect.y, rect.width, rect.height);
}

bool operator== (const Rectangle& r1, const Rectangle& r2) {
    return r1.x      == r2.x &&
           r1.y      == r2.y &&
           r1.width  == r2.width &&
           r1.height == r2.height;
}

bool operator<  (const Rectangle& r1, const Rectangle& r2) {
    return stanfordcpplib::collections::compareTo(r1.x, r2.x, r1.y, r2.y, r1.width, r2.width, r1.height, r2.height);
}
