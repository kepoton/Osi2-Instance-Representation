#ifndef _QUADRATICCONSTRAINT_HPP
#define _QUADRATICCONSTRAINT_HPP

#include "ExpressionConstraint.hpp"
#include "QuadraticExpr.hpp"

#include <exception>

namespace Osi2 {

/*! \brief Class for representing a quadratic constraint

 */
class QuadraticConstraint : public ExpressionConstraint {

    public:
        /// \name Constructors
        //@{

        /// Default constructor
        QuadraticConstraint();

        /// Conversion from ExpressionContraint
        QuadraticConstraint(const ExpressionConstraint& other) : ExpressionConstraint(other){
            if (other.getType() != Constraint::Type::QUADRATIC){
                throw std::invalid_argument("Error, bad conversion");
            }
        }

        /// Constructs a linear constraint with a linear expression and a double
        /// This represents a constraint such as : expr = n
        QuadraticConstraint(const QuadraticExpr& expr, double n);

        /// Constructs a linear constraint with a linear expression and a Range for bounds
        /// This represents a constraint such as : bounds.lower_bound <= expr <= bounds.upper_bounds
        QuadraticConstraint(const QuadraticExpr& expr, const Range& bounds);

        /// Allow conversion from a linear expression to a constraint with infinite bounds. Usefull to build constraints with operator overload
        QuadraticConstraint(const QuadraticExpr& expr);

        /// Copy constructor
        QuadraticConstraint(const QuadraticConstraint& other);

        /// Assignment operator
        QuadraticConstraint& operator=(const QuadraticConstraint& other);

        // Move constructor
        QuadraticConstraint(QuadraticConstraint&& other);

        // Destructor
        virtual ~QuadraticConstraint(){}
        //@}

        /// \name Getters
        //{@

        /// Get the type of the constraint
        Constraint::Type getType() const { return Constraint::Type::QUADRATIC; }
        //@}

        /// \name Setters
        //{@
        virtual void setExpr(const Expression& exp) { this->expr = std::make_shared<QuadraticExpr>(exp); }

        //@}
};

/// \name Operators overload
//{@

/// Lesser than or equal to comparison operator. Used to create constraints in a more mathematical language.
QuadraticConstraint operator<=(const QuadraticConstraint& c, double up);

/// Lesser than or equal to comparison operator. Used to create constraints in a more mathematical language. 
QuadraticConstraint operator<=(double up, const QuadraticConstraint& c);

/// Equal to comparison operator. Used to create constraints in a more mathematical language.
QuadraticConstraint operator==(const QuadraticExpr& exp1, double n);

/// Ostream operator
std::ostream& operator<<(std::ostream& flux, const QuadraticConstraint& constr);
//@}

}

#endif