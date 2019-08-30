#ifndef _VAR_HPP
#define _VAR_HPP

#include <string>
#include <iostream>
#include <functional>
#include <vector>

#include "Range.hpp"

namespace Osi2 {

class LinearExpr;

/*! \brief Class for representing a variable

    A Var is composed of a name, a domaine, a vector of ranges.
    It also as an auto assigned id, used for comparison operators. 

 */

class Var{
    public:
        /// Domaine enumeration
        enum class Domaine { 
            REAL, ///< Real number
            INT, ///< Integer number
            BIN ///< Binary number
            };

        /// \name Constructors
        //{@

        /// Default constructor with default domaine, name and range
        Var(Var::Domaine d = Var::Domaine::REAL);

        /// Constructs a Var with a custom name
        Var(std::string name, Var::Domaine d = Var::Domaine::REAL);

        /// Constructs a Var with a custom name and a vector of ranges
        Var(std::string name, const std::vector<Range>& ranges, Var::Domaine d = Var::Domaine::REAL);

        /// Constructs a Var with a custom name and a range
        Var(std::string name, const Range& range, Var::Domaine d = Var::Domaine::REAL);

        /// Constructs a Var with a vector of ranges
        Var(const std::vector<Range>& ranges, Var::Domaine d = Var::Domaine::REAL);

        /// Constructs a Var with a custom range
        Var(const Range& range, Var::Domaine d = Var::Domaine::REAL);

        /// Copy constructor
        Var(const Var& other);

        /// Assignment operator
        Var& operator=(const Var& other);

        /// Move constructor
        Var(Var&& other);
        //@}

        /// \name Getters
        //{@

        /// Get the name
        const std::string& getName() const { return name; }

        /// Get the domaine
        Var::Domaine getDomaine() const { return domaine; }

        /// Get the vector of ranges (size of 1 in case of a single range)
        const std::vector<Range>& getRanges() const { return ranges; }

        /// Get the ID. Usefull for comparison operations.
        uint32_t getID() const { return id; }

        /// Get the value
        double getValue() const { return value; }
        //@}

        /// \name Setters
        // {@

        /// Set the name
        void setName(const std::string& str) { name = str; }

        /// Set the domaine
        void setDomaine(Var::Domaine new_domaine) { domaine = new_domaine; }

        /// Set the value. Used if you want to store the result of a calculation. Useless for problem representation 
        void setValue(double v) { value = v; }

        /// Add a range to the ranges vector by passing the lower and upper bounds of the range
        void addRange(double low, double up);

        /// Add a range to the ranges vector by passing a Range object
        void addRange(const Range& range);
        //@}

        /// \name Comparison
        //{@

        /// Lesser than comparison
        bool lesser(const Var& other) const;

        /// Equals to comparison
        bool equals(const Var& other) const;
        //@}

    private:
        static uint32_t next_id;

        std::string name; ///< Name of the variable
        std::vector<Range> ranges; ///< Vector of ranges of the variable
        Var::Domaine domaine; ///< Domaine of the variable
        double value; ///< Value of the variable
        uint32_t id; ///< ID of the variable
};

/// \name Comparison
//{@

/// Lesser than comparison
bool lesser(const Var& a, const Var& b);

/// Equals to comparison
bool equals(const Var& a, const Var& b);

//@}

/// \name Operators overload
//{@

/// Lesser than comparison operator overload with native references 
bool operator<(const Var& a, const Var& b);

/// Equal to comparison operator overload with native references
bool operator==(const Var& a, const Var& b);

/// Ostream operator overload
std::ostream& operator<<(std::ostream& flux, const Var& var);
//@}

}

template<> 
struct std::hash< Osi2::Var >{
    uint32_t operator()(const Osi2::Var& v) const {
        return v.getID();
    }
}; 



#endif // _VAR_HPP