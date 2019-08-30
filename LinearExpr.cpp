#include "LinearExpr.hpp"
#include "Model.hpp"

#include <cmath>

namespace Osi2 {

LinearExpr::LinearExpr(){}

LinearExpr::LinearExpr(Var& var){
    addTerm(std::make_shared<LinearTerm>(1.0, var));
}

LinearExpr::LinearExpr(const Expression& e) : Expression(e) {}

LinearExpr::LinearExpr(const LinearTerm& lt){
    addTerm(std::make_shared<LinearTerm>(lt));
}

LinearExpr::LinearExpr(const PackedVector& coefs, Model& m){
    for (const auto& coef : coefs){ // For each coefficient, allocate a LinearTerm and add it to the expression
        addTerm( std::make_shared<LinearTerm>( coef.second, m.getVariableAtIndex(coef.first) ) );
    }
}

LinearExpr::LinearExpr(const LinearExpr& other) : Expression(other) {}

LinearExpr& LinearExpr::operator=(const LinearExpr& other){
    Expression::operator=(other);
    return (*this);
}

LinearExpr::LinearExpr(LinearExpr&& other) : Expression(other) {
}

std::shared_ptr<Expression> LinearExpr::clone() const {
    auto ret_val = std::make_shared<LinearExpr>();

    for (auto t : getTerms()){
        auto temp = dynamic_cast<LinearTerm*>(t.get());
        ret_val->addTerm( std::make_shared<LinearTerm>( temp->coef, temp->var ) );
    }

    return ret_val;
}

void LinearExpr::add(const LinearExpr& expr){
    for ( const auto& term : expr.getTerms() ){
        addTerm(term);
    }
}

void LinearExpr::substract(const LinearExpr& expr){
    for ( const auto& term : expr.getTerms() ){
        if (LinearTerm* t = static_cast<LinearTerm*>(term.get())){
            t->coef = -t->coef;
            addTerm(std::make_shared<LinearTerm>(*t));
        }      
    }
}

void LinearExpr::mult(double scalar){
    for ( auto term : getTerms()){
        LinearTerm* t = static_cast<LinearTerm*>(term.get());
        t->coef*=scalar;
    }
}

void LinearExpr::divide(double scalar){
    for ( auto term : getTerms()){
        LinearTerm* t = static_cast<LinearTerm*>(term.get());
        t->coef/=scalar;
    }
}

void LinearExpr::operator+=(const LinearExpr& expr){
    add(expr);
}

void LinearExpr::operator-=(const LinearExpr& expr){
    substract(expr);
}

void LinearExpr::operator*=(double scalar){
    mult(scalar);
}

void LinearExpr::operator/=(double scalar){
    divide(scalar);
}

void LinearExpr::addTerm(const std::shared_ptr<Term>& t){
    if ( dynamic_cast<LinearTerm*>(t.get()) ){ // Check if the argument is a LinearTerm
        auto spot = terms.insert( t ); // Try to insert the term in the expression

        if ( !spot.second  ){ // If the insertion failed, it means that a term with the same variable is already in the expression
            auto res = spot.first->get()->add(t); // In that case, we add up the 2 terms
            terms.erase(spot.first); // We delete the existing one
            if (res != nullptr) // If the sum is not zero
                terms.insert( res ); // We insert the sum of the 2 terms
        }
    }
}

void LinearExpr::addTerm(double coef, Var& v){
    addTerm(std::make_shared<LinearTerm>(coef, v));
}

std::ostream& operator<<(std::ostream& flux, const LinearExpr& e){
    bool first_it = true;
    for (const auto& x : e.getTerms() ){
        auto term = static_cast<LinearTerm*>(x.get());
        if ( term->coef >= 0 ){
            if ( first_it)
                flux << term->coef; 
            else
                flux << " + " << term->coef; 
        }
        else
            flux << " - " << std::abs(term->coef);

        flux << " * " << term->var << " ";


        first_it = false;
    }

    return flux;
}

LinearExpr sum(const LinearExpr& a, const LinearExpr& b){
    LinearExpr ret_val(a);
    ret_val.add(b);

    return ret_val;
}

LinearExpr diff(const LinearExpr& a, const LinearExpr& b){
    LinearExpr ret_val(a);
    ret_val.substract(b);

    return ret_val;
}

LinearExpr mult(const LinearExpr& e, double scalar){
    LinearExpr ret_val(e);
    ret_val.mult(scalar);

    return ret_val;
}

LinearExpr divide(const LinearExpr& e, double scalar){
    LinearExpr ret_val(e);
    ret_val.divide(scalar);

    return ret_val;
}


LinearExpr operator+(const LinearExpr& a, const LinearExpr& b){
    return sum(a,b);
}


LinearExpr operator-(const LinearExpr& a, const LinearExpr& b){
    return diff(a,b);
}


LinearExpr operator*(double scalar, const LinearExpr& e){
    return mult(e, scalar);
}


LinearExpr operator*(const LinearExpr& e, double scalar){
    return mult(e, scalar);
}

LinearExpr operator/(const LinearExpr& e, double scalar){
    return divide(e, scalar);
}

}