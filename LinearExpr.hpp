#ifndef _LINEAREXPR_HPP
#define _LINEAREXPR_HPP

#include <set>
#include <functional>
#include <iostream>
#include <unordered_map>

#include "Expression.hpp"
#include "Var.hpp"
#include "PackedVector.hpp"

namespace Osi2 {

class Model;

/*! \brief Struct to represent a term in a linear expression 
    
    It inherits Term and is composed of a coefficient.
 */
struct LinearTerm : public Term {
    /// \name Constructors
    //{@

    /// Constructs a LinearTerm with a reference to a Var
    LinearTerm(Var& var) : Term(var), coef(1) {}

    /// Constructs a LinearTerm with a coefficient and a reference to a Var
    LinearTerm(double c, Var& var) : Term(var), coef(c) {}

    /// Copy constructor
    LinearTerm(const LinearTerm& other) : Term(other), coef(other.coef) {}

    /// Assignment operator
    LinearTerm& operator=(const LinearTerm& other){
        Term::operator=(other);
        coef = other.coef;

        return *this;
    }

    /// Move constructor
    LinearTerm(LinearTerm&& other) : Term(other), coef(other.coef) {}

    ///Destructor
    ~LinearTerm() {}
    //@}

    /// \name Operations
    //{@

    /// Sum the LinearTerm with a given LinearTerm
    std::shared_ptr<Term> add(const std::shared_ptr<Term>& term){
        if ( LinearTerm* l_term = dynamic_cast<LinearTerm*>(term.get()) ){
            if (coef + l_term->coef != 0)
                return std::make_shared<LinearTerm>(coef + l_term->coef, this->var);
            else 
                return nullptr;
        }
        else{
            return nullptr;
        }
    }
    //@}

    /// String representation of the linear expression
    std::string toString() const { 
        std::string ret_val = coef < 0 ? std::to_string(coef) : " + "+std::to_string(coef);
        ret_val+="*"+var.getName();

        return ret_val;
    }

    double coef; ///< Coefficient of the term
};

/*! \brief Class for representing a linear expression

 */
class LinearExpr : public Expression {
    public:

        /// \name Constructors
        //{@

        /// Default constructor
        LinearExpr();

        /// Constructor to allow conversion from a Var to a LinearExpr
        LinearExpr(Var& var);

        //LinearExpr(const PackedVector& coefs, std::unordered_map<uint32_t, std::reference_wrapper<Var>> indices_var_inv);

        /// Constructs the linear expression from a given Expression
        LinearExpr(const Expression& e);

        /// Constructs the linear expression from a LinearTerm
        LinearExpr(const LinearTerm& lt);

        //Constructs a linear expression from a vector of coefficients and a model
        LinearExpr(const PackedVector& coefs, Model& m);

        /// Copy constructor
        LinearExpr(const LinearExpr& other);

        /// Assignment operator
        LinearExpr& operator=(const LinearExpr& other);

        /// Move constructor
        LinearExpr(LinearExpr&& other);
        //@}

        /// \name Getters
        //{@

        /// Get the type of the expression
        Expression::Type getType() const { return Expression::Type::LINEAR; }
        

        /// Clones the LinearExpr
        std::shared_ptr<Expression> clone() const;
        //@}

        /// \name Member operations
        //{@

        /// Add a linear expression to the current linear expression
        void add(const LinearExpr& expr);

        /// Substract a linear expression to the current linear expression
        void substract(const LinearExpr& expr);

        /// Multiply the current linear expression by a given scalar
        void mult(double scalar);

        /// Divide the current linear expression by a given scalar
        void divide(double scalar);
        //@}

        
        /// \name Member operators overload
        //{@

        /// Add a linear expression to the current linear expression
        void operator+=(const LinearExpr& expr);

        /// Substract a linear expression to the current linear expression
        void operator-=(const LinearExpr& expr);

        /// Multiply the current linear expression by a given scalar
        void operator*=(double scalar);

        /// Divide the current linear expression by a given scalar
        void operator/=(double scalar);
        //@}

        /// \name Editing functions
        //{@

        /// Adds a LinearTerm to the expression
        void addTerm(const std::shared_ptr<Term>& t);

        /// Constructs and adds a LinearTerm to the expression
        void addTerm(double coef, Var& v);
        //@}

        /// Ostream operator overload
        friend std::ostream& operator<<(std::ostream& flux, const LinearExpr& e);

};

/// \name Operations
//@{

/// Sum of two quadratic expressions
LinearExpr sum(const LinearExpr& a, const LinearExpr& b);

/// Difference of two quadratic expressions
LinearExpr diff(const LinearExpr& a, const LinearExpr& b);

/// Multiplication of a quadratic expression by a scalar
LinearExpr mult(const LinearExpr& e, double scalar);

/// Division of a quadratic expression by a scalar
LinearExpr divide(const LinearExpr& e, double scalar);
//@}

/// \name Arithmetic operator overloads
//{@

/// Sum of two linear expressions
LinearExpr operator+(const LinearExpr& a, const LinearExpr& b);

/// Difference of two linear expressions
LinearExpr operator-(const LinearExpr& a, const LinearExpr& b);

/// Multiplication of a linear expression by a scalar
LinearExpr operator*(double scalar, const LinearExpr& e);

/// Multiplication of a linear expression by a scalar
LinearExpr operator*(const LinearExpr& e, double scalar);

/// Division of a linear expression by a scalar
LinearExpr operator/(const LinearExpr& e, double scalar);

//@}

}


#endif // _LINEAREXPR_HPP