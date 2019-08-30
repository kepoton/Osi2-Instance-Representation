#ifndef _EXPRESSIONCONSTRAINT_HPP
#define _EXPRESSIONCONSTRAINT_HPP

#include <cstdint>
#include <string>
#include <memory>

#include "Constraint.hpp"
#include "Range.hpp"
#include "Expression.hpp"

namespace Osi2 {

/*! \brief Base abstract class to define different types of constraints

 */
class ExpressionConstraint : public Constraint {
    public:
        /// Constraint type enumeration
        enum class Format { 
            LE, ///< Lesser than or Equal to
            EQ ///< Equal to
        };

        /// \name Setters
        //{@

        /// Assignment operator
        ExpressionConstraint& operator=(const ExpressionConstraint& other);

        /// \name Getters
        //{@

        /// Get the expression inside the constraint
        const Expression& getExpr() const { return *expr.get(); }

        /// Pure virtual fonction to get the type of the constraint
        virtual Constraint::Type getType() const = 0;

        /// String representation of the constraint
        virtual std::string toString() const;

        /// Get the lower bound of the constraint
        double getLowerBound() const { return bounds.lower_bound; }

        /// Get the upper bound of the constraint
        double getUpperBound() const { return bounds.upper_bound; }

        /// Get the Range representing the bounds of the constraint
        const Range& getBounds() const { return bounds; }
        //@}

        /// Get the format of the constraint
        ExpressionConstraint::Format getFormat() const { return format; }


        /// \name Setters
        //{@

        /// Set the lower bound of the constraint
        void setLowerBound(double lower) { bounds.lower_bound = lower; }

        /// Set the upper bound of the constraint
        void setUpperBound(double upper) { bounds.upper_bound = upper; }

        /// Set the Range representing the bounds of the constraint
        void setBounds(const Range& range) { bounds = range; }

        /// Set the format of the constraint
        void setFormat(ExpressionConstraint::Format f) { format = f; }

        /// Set the expression inside the constraint
        virtual void setExpr(const Expression& exp) = 0;// { expr = std::make_shared<Expression>(exp); }
        //@}

        /// \name Iterator functions
        //{@

        /// Get the begin iterator of the constraint. The constraint iterator only runs through the expression.
        std::set<std::shared_ptr<Term>>::const_iterator begin() const { return std::begin(expr->getTerms()); }

        /// Get the end iterator of the constraint. The constraint iterator only runs through the expression.
        std::set<std::shared_ptr<Term>>::const_iterator end() const { return std::end(expr->getTerms()); }
        //@}

    protected:

        /// \name Constructors
        //{@

        /// Default constructor
        ExpressionConstraint();

        /// Constructs a constraint with a name
        ExpressionConstraint(const std::string& name);

        /// Copy constructor
        ExpressionConstraint(const ExpressionConstraint& other);

        /// Move constructor
        ExpressionConstraint(ExpressionConstraint&& other);
        //@}

        std::shared_ptr<Expression> expr;

    private:
        Range bounds; ///< The bounds of the constraint
        ExpressionConstraint::Format format; ///< The format of the constraint
};

}


#endif