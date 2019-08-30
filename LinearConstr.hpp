#ifndef _LINEARCONSTR_HPP
#define _LINEARCONSTR_HPP

#include "ExpressionConstraint.hpp"
#include "LinearExpr.hpp"
#include "PackedVector.hpp"

#include <limits>

namespace Osi2 {

class Model;

/*! \brief Class for representing a linear constraint

 */
class LinearConstr : public ExpressionConstraint {
    public:

        /// \name Constructors
        //@{

        /// Default constructor
        LinearConstr();

        /// Conversion from ExpressionContraint
        LinearConstr(const ExpressionConstraint& other) : ExpressionConstraint(other){
            if (other.getType() != Constraint::Type::LINEAR){
                throw std::invalid_argument("Error, bad conversion");
            }
        }

        /// Constructs a linear constraint with a linear expression and a double
        /// This represents a constraint such as : expr = n
        LinearConstr(const LinearExpr& expr, double n);

        /// Constructs a linear constraint with a linear expression and a Range for bounds
        /// This represents a constraint such as : bounds.lower_bound <= expr <= bounds.upper_bounds
        LinearConstr(const LinearExpr& expr, const Range& bounds);

        /// Constructs a linear constraint via a PackedVector of coefficients, a range, and a reference to a Var container
        LinearConstr(const PackedVector& coefs, const Range& range, Model& m);

        /// Allow conversion from a linear expression to a constraint with infinite bounds. Usefull to build constraints with operator overload
        LinearConstr(const LinearExpr& expr);

        /// Copy constructor
        LinearConstr(const LinearConstr& other);

        /// Assignment operator
        LinearConstr& operator=(const LinearConstr& other);

        // Move constructor
        LinearConstr(LinearConstr&& other);

        // Destructor
        virtual ~LinearConstr(){}

        //@}

        /// \name Getters
        //{@

        /// Get the type of the constraint
        Constraint::Type getType() const { return Constraint::Type::LINEAR; }
        //@}

        /// \name Setters
        //{@
        virtual void setExpr(const Expression& exp) { this->expr = std::make_shared<LinearExpr>(exp); }

        //@}



    private:
        //LinearExpr expr; ///< The expression inside the constraint
};

/// \name Operators overload
//{@

/// Lesser than or equal to comparison operator. Used to create constraints in a more mathematical language.
LinearConstr operator<=(const LinearConstr& c, double up);

/// Lesser than or equal to comparison operator. Used to create constraints in a more mathematical language. 
LinearConstr operator<=(double up, const LinearConstr& c);

/// Equal to comparison operator. Used to create constraints in a more mathematical language.
LinearConstr operator==(const LinearConstr& exp1, double n);

/// Ostream operator
std::ostream& operator<<(std::ostream& flux, const LinearConstr& constr);
//@}

}


#endif // _LINEARCONSTR_HPP