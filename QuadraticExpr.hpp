#ifndef _QUADRATICEXPR_HPP
#define _QUADRATICEXPR_HPP

#include "Expression.hpp"
#include "LinearExpr.hpp"

#include <array>

namespace Osi2 {

/*! \brief Struct to represent a term in a quadratic expression 
    
    It inherits Term and is composed of an array of 3 coefficients.
 */

struct QuadraticTerm : public Term {

    /// \name Constructors
    //{@

    /// Constructs a QuadraticTerm with 3 coefficients and a reference to a Var
    QuadraticTerm(double c2, double c1, double c0, Var& var) : Term(var), coefs({c0, c1, c2}) {}

    /// Constructs a QuadraticTerm with an array of 3 coefficients and a reference to a Var
    QuadraticTerm(const std::array<double, 3>& coefficients, Var& var) : Term(var), coefs({coefficients[2], coefficients[1], coefficients[0]}) {}

    /// Copy constructor
    QuadraticTerm(const QuadraticTerm& other) : Term(other), coefs(other.coefs) {}

    /// Assignment operator
    QuadraticTerm& operator=(const QuadraticTerm& other){
        Term::operator=(other);
        coefs = other.coefs;

        return *this;
    }

    /// Move constructor
    QuadraticTerm(QuadraticTerm&& other) : Term(other.var) {
        std::swap(coefs, other.coefs);
    }
    //@}

    /// Sum the QuadraticTerm with a given QuadraticTerm
    std::shared_ptr<Term> add(const std::shared_ptr<Term>& term){
        if ( QuadraticTerm* l_term = dynamic_cast<QuadraticTerm*>(term.get()) ){
            if (coefs[0] + l_term->coefs[0] == 0.0 && coefs[1] + l_term->coefs[1] == 0.0 && coefs[2] + l_term->coefs[2] == 0.0)
                return nullptr;
            else
                return std::make_shared<QuadraticTerm>(coefs[2] + l_term->coefs[2], coefs[1] + l_term->coefs[1], coefs[0] + l_term->coefs[0], this->var);
        }
        else
            return nullptr;
    }
    //@}

    /// String representation of the linear expression
    std::string toString() const { 
        std::string ret_val = "";
        for (int i = 2; i >= 0; i--){
            ret_val += coefs[i] < 0 ? std::to_string(coefs[i]) : "+"+std::to_string(coefs[i]);
            if (i > 0)
                ret_val+="*"+var.getName()+"^"+std::to_string(i);
            else
                ret_val+="*"+var.getName();
            
        }
        return ret_val;
    }

    std::array<double, 3> coefs;
};

/*! \brief Class for representing a quadratic expression

 */
class QuadraticExpr : public Expression {
    public:
        /// \name Constructors
        //{@

        /// Default constructor
        QuadraticExpr();

        /// Constructs the quadratic expression from a given Expression
        QuadraticExpr(const Expression& e);

        /// Constructs the quadratic expression from a QuadraticTerm
        QuadraticExpr(const QuadraticTerm& qt);

        /// Copy constructor
        QuadraticExpr(const QuadraticExpr& other);

        /// Assignment operator
        QuadraticExpr& operator=(const QuadraticExpr& other);

        /// Move constructor
        QuadraticExpr(QuadraticExpr&& other);
        //@}

        /// \name Getters
        //{@

        /// Returns the type of the expression, here a quadratic expression
        Expression::Type getType() const { return Expression::Type::QUADRATIC; }

        /// Clones the QuadraticExpression
        std::shared_ptr<Expression> clone() const;
        //@}

        /// \name Operations
        //{@

        /// Add a quadratic expression to the current quadratic expression
        void add(const QuadraticExpr& expr);

        /// Substract a quadratic expression to the current quadratic expression
        void substract(const QuadraticExpr& expr);

         /// Multiply the current quadratic expression by a given scalar
        void mult(double scalar);

        /// Divide the current quadratic expression by a given scalar
        void divide(double scalar);
        //@}

        /// \name Member operators overload
        //{@

        /// Add a quadratic expression to the current quadratic expression
        void operator+=(const QuadraticExpr& expr);

        /// Substract a quadratic expression to the current quadratic expression
        void operator-=(const QuadraticExpr& expr);

        /// Add a linear expression to the current quadratic expression
        /*void operator+=(const LinearExpr& expr);

        /// Substract a linear expression to the current quadratic expression
        void operator-=(const LinearExpr& expr);*/

        /// Multiply the current quadratic expression by a given scalar
        void operator*=(double scalar);

        /// Divide the current quadratic expression by a given scalar
        void operator/=(double scalar);
        //@}

        /// \name Editing functions
        //{@

        /// Add a QuadraticTerm to the expression
        void addTerm(const std::shared_ptr<Term>& t);

        /// Add a QuadraticTerm to the expression
        void addTerm(double c2, double c1, double c0, Var& var);
        //@}

        /// Ostream operator overload
        friend std::ostream& operator<<(std::ostream& flux, const QuadraticExpr& e);
};

/// \name Arithmetic operator overloads
//{@

/// Sum of two quadratic expressions
QuadraticExpr sum(const QuadraticExpr& a, const QuadraticExpr& b);

/// Difference of two quadratic expressions
QuadraticExpr diff(const QuadraticExpr& a, const QuadraticExpr& b);

/// Multiplication of a quadratic expression by a scalar
QuadraticExpr mult(const QuadraticExpr& e, double scalar);

/// Division of a quadratic expression by a scalar
QuadraticExpr divide(const QuadraticExpr& e, double scalar);

/// Sum of two quadratic expressions
QuadraticExpr operator+(const QuadraticExpr& a, const QuadraticExpr& b);

/// Difference of two quadratic expressions
QuadraticExpr operator-(const QuadraticExpr& a, const QuadraticExpr& b);

/// Multiplication of a quadratic expression by a scalar
QuadraticExpr operator*(double scalar, const QuadraticExpr& e);

/// Multiplication of a quadratic expression by a scalar
QuadraticExpr operator*(const QuadraticExpr& e, double scalar);

/// Division of a quadratic expression by a scalar
QuadraticExpr operator/(const QuadraticExpr& e, double scalar);

/*QuadraticExpr operator+(const QuadraticExpr& a, const LinearExpr& b);

QuadraticExpr operator+(const LinearExpr& a, const QuadraticExpr& b);*/

//@}

}

#endif