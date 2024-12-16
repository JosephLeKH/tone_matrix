#pragma once

#include "Sample.h"
#include "StringInstrument.h"

/* Type that maintains a Tone Matrix, reacts to mouse movement,
 * handles graphics, and sends data to the computer speakers.
 */
class ToneMatrix {
public:
    /* Creates a Tone Matrix where each light in the matrix has
     * size lightSize x lightSize. The Tone Matrix itself is always
     * a 16 x 16 grid; the lightSize parameter just determines the
     * size, in pixels, of each light in the grid.
     */
    ToneMatrix(int lightSize);

    /* Frees all memory allocated by the ToneMatrix type. */
    ~ToneMatrix();

    /* Reacts to the mouse being pressed at a given location.
     * Specifically, this toggles the state of the light under the
     * mouse.
     */
    void mousePressed(int mouseX, int mouseY);

    /* Reacts to the mouse being dragged at a given location (moved
     * while pressed). Specifically, this updates the light under the
     * mouse to match the state of the light where the mouse was
     * first pressed.
     */
    void mouseDragged(int mouseX, int mouseY);

    /* Draws the Tone Matrix to the screen. */
    void draw() const;

    /* Produces the next sound sample from the Tone Matrix. */
    Sample nextSample();

private:
    int _lightDimension;
    bool* _grid = nullptr;
    StringInstrument* _instruments = nullptr;
    int _lastClicked;
    int _samples;
    int _sampleCol;
};
