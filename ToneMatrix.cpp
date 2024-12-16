/**
 * Joseph Le
 * ToneMatrix.cpp
 * 11/8/2024
 *
 * This class allows users to create a grid of lights that correspond to
 * musical notes, enabling interaction via mouse input to toggle notes
 * and visualize their state. It also generates sound samples based on
 * the activated lights.
 */

#include "ToneMatrix.h"
#include "Demos/DrawRectangle.h"
#include <cmath>
using namespace std;

/* Color of a light that is off and that is on. */
const Color kLightOffColor( 64,  64,  64);
const Color kLightOnColor (192, 192, 192);

/* Frequencies of the notes in the Tone Matrix. You will need to use the
 * kFrequencies array when creating the array of instruments.
 *
 * For those of you who are musically inclined: the base frequency is
 * chosen to be a low C. The remaining notes are then repeated major
 * pentatonic scales stacked on top. To see why, note that one half step
 * corresponds to multiplying the frequency by the twelfth root of two.
 * Therefore, multiplying a frequency by pow(2, n / 12.0) corresponds
 * to taking the note n half-steps above the base frequency.
 *
 * Feel free to tinker and tweak these frequencies as an extension.
 * However, don't modify them when working on the base assignment;
 * our test cases make some assumptions based on how they work.
 */
const double kBaseFrequency = 220.0 * pow(2, 3.0 / 12.0);
const double kFrequencies[16] = {
    kBaseFrequency * pow(2, 36.0 / 12.0),
    kBaseFrequency * pow(2, 33.0 / 12.0),
    kBaseFrequency * pow(2, 31.0 / 12.0),
    kBaseFrequency * pow(2, 28.0 / 12.0),
    kBaseFrequency * pow(2, 26.0 / 12.0),
    kBaseFrequency * pow(2, 24.0 / 12.0),
    kBaseFrequency * pow(2, 21.0 / 12.0),
    kBaseFrequency * pow(2, 19.0 / 12.0),
    kBaseFrequency * pow(2, 16.0 / 12.0),
    kBaseFrequency * pow(2, 14.0 / 12.0),
    kBaseFrequency * pow(2, 12.0 / 12.0),
    kBaseFrequency * pow(2,  9.0 / 12.0),
    kBaseFrequency * pow(2,  7.0 / 12.0),
    kBaseFrequency * pow(2,  4.0 / 12.0),
    kBaseFrequency * pow(2,  2.0 / 12.0),
    kBaseFrequency * pow(2,  0.0 / 12.0),
};

/**
 * @brief Constructs a ToneMatrix with a specified light size.
 *
 * Initializes the light dimension, grid, and array of string instruments.
 *
 * @param lightSize The size of each light in the matrix.
 */
ToneMatrix::ToneMatrix(int lightSize) {
    _samples = -1;
    _sampleCol = 0;
    _lightDimension = lightSize;
    _grid = new bool[16 * 16]();
    _instruments = new StringInstrument[16];

    for (int i = 0; i < 16; i++)
    {
        _instruments[i] = StringInstrument(kFrequencies[i]);
    }
}

/**
 * @brief Destructor for ToneMatrix.
 *
 * Cleans up dynamically allocated memory for the grid and instruments.
 */
ToneMatrix::~ToneMatrix() {
    delete[] _grid;
    delete[] _instruments;
}

/**
 * @brief Handles mouse press events to toggle the light at the clicked position.
 *
 * Updates the state of the grid based on mouse coordinates.
 *
 * @param mouseX The x-coordinate of the mouse click.
 * @param mouseY The y-coordinate of the mouse click.
 */
void ToneMatrix::mousePressed(int mouseX, int mouseY) {
    int r = mouseY / _lightDimension;
    int c = mouseX / _lightDimension;

    _grid[16 * r + c] = !_grid[16 * r + c];
    _lastClicked = _grid[16 * r + c];
}

/**
 * @brief Handles mouse drag events to change the state of lights.
 *
 * Toggles the light at the current mouse position based on the last clicked state.
 *
 * @param mouseX The current x-coordinate of the mouse.
 * @param mouseY The current y-coordinate of the mouse.
 */
void ToneMatrix::mouseDragged(int mouseX, int mouseY) {
    int r = mouseY / _lightDimension;
    int c = mouseX / _lightDimension;

    _grid[16 * r + c] = _lastClicked;
}

/**
 * @brief Draws the ToneMatrix grid.
 *
 * Iterates through the grid and draws each light in its current state.
 */
void ToneMatrix::draw() const {
    Rectangle rect;
    rect.width = _lightDimension;
    rect.height = _lightDimension;

    for (int i = 0; i < 16; i++)
    {
        for (int k = 0; k < 16; k++)
        {
            rect.x = _lightDimension * k;
            rect.y = _lightDimension * i;
            if (_grid[16 * i + k])
            {
                drawRectangle(rect, kLightOnColor);
            }
            else
            {
                drawRectangle(rect, kLightOffColor);
            }
        }
    }
}

/**
 * @brief Generates the next audio sample.
 *
 * Increments the sample counter and generates a new sample based on the active instruments.
 *
 * @return The next audio sample as a floating-point value.
 */
Sample ToneMatrix::nextSample() {
    _samples++;

    if (_samples % 8192 == 0)
    {
        for (int i = 0; i < 16; i++)
        {
            if (_grid[16 * i + _sampleCol])
            {
                _instruments[i].pluck();
            }
        }
        _sampleCol = (_sampleCol + 1) % 16;
    }

    Sample sum = 0.0;
    for (int i = 0; i < 16; i++)
    {
        sum += _instruments[i].nextSample();
    }

    return sum;
}
