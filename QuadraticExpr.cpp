#include "QuadraticExpr.hpp"

namespace Osi2 {

QuadraticExpr::QuadraticExpr(){}

QuadraticExpr::QuadraticExpr(const Expression& e) : Expression(e) {}

QuadraticExpr::QuadraticExpr(const QuadraticTerm& qt) {
    addTerm(std::make_shared<QuadraticTerm>(qt));
}

QuadraticExpr::QuadraticExpr(const QuadraticExpr& other) : Expression(other) {}

QuadraticExpr& QuadraticExpr::operator=(const QuadraticExpr& other){
    Expression::operator=(other);
    return (*this);
}

QuadraticExpr::QuadraticExpr(QuadraticExpr&& other) : Expression(other) {}

std::shared_ptr<Expression> QuadraticExpr::clone() const {
    auto ret_val = std::make_shared<QuadraticExpr>();

    for (auto t : getTerms()){
        auto temp = dynamic_cast<QuadraticTerm*>(t.get());
        ret_val->addTerm( std::make_shared<QuadraticTerm>( temp->coefs[2], temp->coefs[1], temp->coefs[0], temp->var ) );
    }

    return ret_val;
}

void QuadraticExpr::add(const QuadraticExpr& expr){
    for ( const auto& term : expr.getTerms() ){
        addTerm(term);
    }
}

void QuadraticExpr::substract(const QuadraticExpr& expr){
    for ( const auto& term : expr.getTerms() ){
        if (QuadraticTerm* t = static_cast<QuadraticTerm*>(term.get())){
            t->coefs[0] = -t->coefs[0];
            t->coefs[1] = -t->coefs[1];
            t->coefs[2] = -t->coefs[2];
            addTerm(std::make_shared<QuadraticTerm>(*t)); //TODO check that
        }      
    }
}

void QuadraticExpr::mult(double scalar){
    for ( auto term : getTerms()){
        QuadraticTerm* t = static_cast<QuadraticTerm*>(term.get());
        t->coefs[0]*=scalar;
        t->coefs[1]*=scalar;
        t->coefs[2]*=scalar;
    }
}

void QuadraticExpr::divide(double scalar){
    for ( auto term : getTerms()){
        QuadraticTerm* t = static_cast<QuadraticTerm*>(term.get());
        t->coefs[0]/=scalar;
        t->coefs[0]/=scalar;
        t->coefs[0]/=scalar;
    }
}

void QuadraticExpr::operator+=(const QuadraticExpr& expr){
    add(expr); 
}

void QuadraticExpr::operator-=(const QuadraticExpr& expr){
    substract(expr);
}


void QuadraticExpr::operator*=(double scalar){
    mult(scalar);
}

void QuadraticExpr::operator/=(double scalar){
    divide(scalar);
}

void QuadraticExpr::addTerm(const std::shared_ptr<Term>& t){
    if ( dynamic_cast<QuadraticTerm*>(t.get())){ // Check if the argument is a QuadraticTerm
        auto spot = terms.insert( t ); // Try to insert the term in the expression

        if ( !spot.second  ){ // If the insertion failed, it means that a term with the same variable is already in the expression
            auto res = spot.first->get()->add(t); // In that case, we add up the 2 terms
            terms.erase(spot.first); // We delete the existing one
            if (res != nullptr) // If the sum is not zero
                terms.insert( res ); // We insert the sum of the 2 terms
        }
    }

}

void QuadraticExpr::addTerm(double c2, double c1, double c0, Var& var){
    addTerm(std::make_shared<QuadraticTerm>( c2, c1, c0 , var));
}

std::ostream& operator<<(std::ostream& flux, const QuadraticExpr& e){
    bool first_it = true;
    for (const auto& x : e.getTerms() ){
        auto term = static_cast<QuadraticTerm*>(x.get());
        if ( term->coefs[2] >= 0 ){
            if ( first_it)
                flux << term->coefs[2]; 
            else
                flux << " + " << term->coefs[2]; 
        }
        else 
            flux << " - " << std::abs(term->coefs[2]);

        flux << " * " << term->var << "^2 ";


        if (term->coefs[1] >= 0 ){
            flux << " + " << term->coefs[1];
        }
        else 
            flux << " - " << std::abs(term->coefs[1]);
        
        flux << " * " << term->var << " ";


        if (term->coefs[0] >= 0 ){
            flux << " + " << term->coefs[0];
        }
        else 
            flux << " - " << std::abs(term->coefs[0]);

        first_it = false;
    }

    return flux;
}


QuadraticExpr sum(const QuadraticExpr& a, const QuadraticExpr& b){
    QuadraticExpr ret_val(a);
    ret_val.add(b);

    return ret_val;
}

QuadraticExpr diff(const QuadraticExpr& a, const QuadraticExpr& b){
    QuadraticExpr ret_val(a);
    ret_val.substract(b);

    return ret_val;
}

QuadraticExpr mult(const QuadraticExpr& e, double scalar){
    QuadraticExpr ret_val(e);
    ret_val.mult(scalar);

    return ret_val;
}

QuadraticExpr divide(const QuadraticExpr& e, double scalar){
    QuadraticExpr ret_val(e);
    ret_val.divide(scalar);

    return ret_val;
}

QuadraticExpr operator+(const QuadraticExpr& a, const QuadraticExpr& b){
    return sum(a, b);
}

QuadraticExpr operator-(const QuadraticExpr& a, const QuadraticExpr& b){
    return diff(a,b);
}

QuadraticExpr operator*(double scalar, const QuadraticExpr& e){
    return mult(e, scalar);
}

QuadraticExpr operator*(const QuadraticExpr& e, double scalar){
    return mult(e, scalar);
}

QuadraticExpr operator/(const QuadraticExpr& e, double scalar){
    return divide(e,scalar);
}


}