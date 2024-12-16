#pragma once

#include "GUI/MemoryDiagnostics.h"

/* Type representing a single sound sample. We use this type instead of the
 * more familiar 'double' for two reasons:
 *
 * 1. Many real-world audio processing libraries (e.g. the Synthesis Toolkit)
 *    use custom types to handle the diversity of hardware devices and
 *    operating systems available.
 *
 * 2. By using the Sample type, we can hook into SimpleTest's memory diagnostics
 *    system to make it easier to identify memory leaks and memory errors.
 *
 * You can treat Sample as though it's a double for pretty much any purpose;
 * it behaves exactly like a double.
 */
class Sample {
public:
    Sample() = default;
    Sample(double value);

    operator double& ();
    operator double  () const;

    TRACK_ALLOCATIONS_OF(Sample);

private:
    double _value;
};

/* Allow for fuzzier equality checks than what SimpleTest would normally do. */
bool operator== (const Sample& lhs, const Sample& rhs);
bool operator!= (const Sample& lhs, const Sample& rhs);
