#pragma once

#include "GUI/Color.h"
#include "Rectangle.h"
#include <functional>
#include <ostream>
using MiniGUI::Color;

using DrawFunction = std::function<void(const Rectangle&, Color)>;

void drawRectangle(const Rectangle& bounds, Color color);
void setDrawFunction(DrawFunction fn);
