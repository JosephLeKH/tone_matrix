#include "Sample.h"
#include <cmath>
using namespace std;

Sample::Sample(double value) : _value(value) {

}

Sample::operator double& () {
    return _value;
}

Sample::operator double () const {
    return _value;
}

bool operator== (const Sample& lhs, const Sample& rhs) {
    static const double kTolerance = 1e-8;
    return fabs(lhs - rhs) <= kTolerance;
}

bool operator!= (const Sample& lhs, const Sample& rhs) {
    return !(lhs == rhs);
}
