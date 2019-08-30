#ifndef _EXPRESSION_HPP
#define _EXPRESSION_HPP

#include <set>
#include <iostream>
#include <memory>
#include <functional>
#include <string>

#include "Var.hpp"


namespace Osi2 {

/*! \brief Abstract struct that represents a generic term in an Expression

 */

struct Term {

    /// \name Constructors
    //{@

    /// Constructs a term with a given Var
    Term(Var& v) : var(v) {}

    /// Copy constructor
    Term(const Term& other) : var(other.var) {}

    /// Assignment operator
    Term& operator=(const Term& other){
        var = other.var;

        return *this;
    }

    /// Move operator
    Term(Term&& other) : var(other.var) {}

    /// Virtual destructor
    virtual ~Term() {}
    //@}

    /// \name Operations
    //{@

    /// Pure virtual method to sum two Term
    virtual std::shared_ptr<Term> add(const std::shared_ptr<Term>& t) = 0 ;
    //@}

    /// String representation of the Term
    virtual std::string toString() const = 0;

    Var& var; /// Reference to the Var associated
};

/// \name Term comparison operators
//{@

/// Lesser than comparison operator
bool operator<(const Term& a, const Term& b);

/// Equal to comparison operator
bool operator==(const Term& a, const Term& b);

/// Lesser than comparison operator
bool operator<(const std::shared_ptr<Term>& a, const std::shared_ptr<Term>& b);

/// Equal to comparison operator
bool operator==(const std::shared_ptr<Term>& a, const std::shared_ptr<Term>& b);

//@}

}

template<>
struct std::less< std::shared_ptr<Osi2::Term> > {
    bool operator()(const std::shared_ptr<Osi2::Term> &lhs, const std::shared_ptr<Osi2::Term> &rhs) const 
    {
        return lhs < rhs;
    }
};

namespace Osi2 {

/*! \brief Base abstract class for mathemical expression
*/

class Expression {

    public:
        /// Defines the type of the expression
        enum class Type { 
            LINEAR, /// Linear expression
            QUADRATIC, // Quadratic expression
        };

        /// \name Constructors
        //{@

        /// Default constructor
        Expression();

        /// Copy constructor
        Expression(const Expression& other);

        /// Assignment operator
        Expression& operator=(const Expression& other);

        /// Move constructor
        Expression(Expression&& other);

        /// Virtual destructor
        virtual ~Expression() {}

        //@}

        /// \name Iterator functions
        //{@

        /// Get the begin iterator of the expression
        std::set<std::shared_ptr<Term>>::const_iterator begin() const { return std::begin(terms); }

        /// Get the end iterator of the expression
        std::set<std::shared_ptr<Term>>::const_iterator end() const { return std::end(terms); }
        //@}

        /// \name Getters
        //{@

        /// Get the set containing the terms of the expression
        const std::set<std::shared_ptr<Term>>& getTerms() const { return terms; }

        /// Get the type of the expression
        virtual Expression::Type getType() const = 0;

        /// Clones the Expression
        virtual std::shared_ptr<Expression> clone() const = 0;

        /// Check if the current linear expression is equivalent to a given linear expression
        bool equals(const Expression& exp) const;

        /// String representation of the Expression
        virtual std::string toString() const;

        //@}

        /// \ Editing functions
        //{@

        /// Pure virtual method to add a term to the expression
        virtual void addTerm(const std::shared_ptr<Term>& t) = 0; // This method can probably be templated
        //@}

    protected:
        std::set<std::shared_ptr<Term>> terms; ///< Container for the terms of the expression    
};

//std::ostream& operator<<(std::ostream& flux, const Expression& e);

}

#endif