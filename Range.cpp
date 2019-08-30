#include "Range.hpp"

#include <algorithm>
#include <stdexcept>

namespace Osi2 {

double Range::POSITIVE_INFINITY = std::numeric_limits<double>::infinity();
double Range::NEGATIVE_INFINITY = -1.0 * Range::POSITIVE_INFINITY;

Range::Range() {
    lower_bound = Range::NEGATIVE_INFINITY;
    upper_bound = Range::POSITIVE_INFINITY;
}

Range::Range(double lower, double upper) {
    if ( lower > upper )
        throw std::invalid_argument("Lower bound larger than upper bound");

    lower_bound = lower;
    upper_bound = upper;
}

Range::Range(double exact) {
    lower_bound = exact;
    upper_bound = exact;
}

Range::Range(const Range& other){
    lower_bound = other.lower_bound;
    upper_bound = other.upper_bound;
}

Range::Range(Range&& other){
    lower_bound = other.lower_bound;
    upper_bound = other.upper_bound;
}

Range& Range::operator=(const Range& other){
    lower_bound = other.lower_bound;
    upper_bound = other.upper_bound;

    return (*this);
}

bool Range::overlap(const Range& other){
    return (other.lower_bound >= lower_bound && other.lower_bound <= upper_bound) ||
            (other.upper_bound >= lower_bound && other.upper_bound <= upper_bound);
}

bool Range::merge(const Range& other){
    bool ret_val = false;

    if (overlap(other)){
        lower_bound = std::min(lower_bound, other.lower_bound);
        upper_bound = std::max(upper_bound, other.upper_bound);

        ret_val = true;
    }

    return ret_val;
}

}
