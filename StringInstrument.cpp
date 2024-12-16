#include "Demos/AudioSystem.h"
#include "StringInstrument.h"
#include "error.h"
using namespace std;


/* Constructor: StringInstrument
 * -----------------------------
 * Initializes a StringInstrument object with a specified frequency. The waveform buffer size is calculated based on
 * the audio sample rate. If the frequency is zero or negative, or if the computed buffer length is invalid, an error
 * is raised.
 */
StringInstrument::StringInstrument(double frequency) {
    if (frequency <= 0)

        {
            error("Frequency can't be less than or equal to 0!");
        }

    _length = (int) (AudioSystem::sampleRate() / frequency);

    if (_length <= 1)
    {
        error("Length of the array is too small (0 or 1");
    }

    _cursor = 0;
    _waveform = new Sample[_length]();

}


/* Destructor: ~StringInstrument
 * -----------------------------
 * Cleans up the memory allocated for the waveform buffer when the StringInstrument is destroyed.
 */
StringInstrument::~StringInstrument() {
    delete[] _waveform;
}

/*
 * Simulates the action of plucking the string by resetting the cursor to the beginning and
 * initializing the waveform with alternating positive and negative values.
 */
void StringInstrument::pluck() {
    _cursor = 0;

        for (int i = 0; i < _length / 2; i++)
    {
        _waveform[i] = 0.05;
    }
    for (int i =  _length / 2; i < _length; i++)
    {
        _waveform[i] = -0.05;
    }
}

/*
 * Generates the next audio sample in the waveform. It returns the sample at the current cursor
 * position, calculates the next value with a simple averaging filter to simulate decay, and
 * advances the cursor in the buffer.
 */
Sample StringInstrument::nextSample() {
    Sample output = _waveform[_cursor];

    int next = (_cursor + 1) % _length;
    _waveform[_cursor] = (_waveform[_cursor] + _waveform[next]) / 2 * 0.995;

            _cursor = next;

    return output;
}

/* * * * * Special Functions Below This Point * * * * */

#include <algorithm>

/* Copy constructor for StringInstrument. This is called automatically by C++
 * if you need to make a copy of a StringInstrument; say, if you were to pass
 * one into a function by value. Take CS106L for more details about how this
 * works.
 */
StringInstrument::StringInstrument(const StringInstrument& rhs) {
    /* Not normally part of a copy constructor, this line is designed
     * to ensure that if you haven't implemented StringInstrument,
     * you don't get weird and wild crashes.
     */
    if (rhs._waveform == nullptr) {
        return;
    }

    _length   = rhs._length;
    _waveform = new Sample[_length];
    _cursor   = rhs._cursor;

    for (int i = 0; i < _length; i++) {
        _waveform[i] = rhs._waveform[i];
    }
}

/* Assignment operator for StringInstrument. This is called automatically by
 * C++ when you assign one StringInstrument to another. Take CS106L for more
 * details about how this works. This specific implementation uses an idiom
 * called "copy-and-swap."
 */
StringInstrument& StringInstrument::operator =(StringInstrument rhs) {
    swap(_length,   rhs._length);
    swap(_waveform, rhs._waveform);
    swap(_cursor,   rhs._cursor);
    return *this;
}
