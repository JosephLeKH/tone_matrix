#include "DrawRectangle.h"
#include "error.h"
using namespace std;

namespace {
    DrawFunction theDrawFunction;
}

void drawRectangle(const Rectangle& bounds, Color color) {
    if (!theDrawFunction) {
        error("drawRectangle() was called without a RectangleCatcher set up to catch the rectangles. Make sure to create a RectangleCatcher when testing the ToneMatrix::draw() function.");
    }

    theDrawFunction(bounds, color);
}

void setDrawFunction(DrawFunction fn) {
    theDrawFunction = fn;
}
