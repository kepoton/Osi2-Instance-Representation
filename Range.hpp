#ifndef _RANGE_HPP
#define _RANGE_HPP

#include <limits>

namespace Osi2 {

/*! \brief Struct for representing a range, with an upper and a lower bound
    
    The purpose of this struct is to represent the bounds of variables and constraints.
    It uses the C++ standard library to represent positive and negative infinity. 
 */

struct Range {

    /// \name Constructors
    //{@

    /// Default constructor ]-inf, +inf[
    Range();

    /// Constructs by passing a lower and an upper bound
    Range(double lower, double upper);

    /// Constructs by passing a single value, that will be both the lower and the upper bounds
    Range(double exact);

    /// Copy constructor
    Range(const Range& other);

    /// Assignment operator
    Range& operator=(const Range& other);

    /// Move constructor
    Range(Range&& other);
    //@}


    /// Checks if the range intersects with an other range
    bool overlap(const Range& other);

    /// Merges two overlapping ranges. The result is the union of the two ranges.
    bool merge(const Range& other);


    /// Representation of positive infinity 
    static double POSITIVE_INFINITY;
    /// Representation of negative infinity 
    static double NEGATIVE_INFINITY;

    
    /// Lower bound of the range
    double lower_bound;
    ///Upper bound of the range
    double upper_bound;
};

}

#endif //_RANGE_HPP