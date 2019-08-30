#include "Expression.hpp"

#include "LinearExpr.hpp"
#include "QuadraticExpr.hpp"

namespace Osi2 {

bool operator<(const Term& a, const Term& b){
    return a.var < b.var;
}

bool operator==(const Term& a, const Term& b){
    return a.var == b.var;
}

bool operator<(const std::shared_ptr<Term>& a, const std::shared_ptr<Term>& b){
    return (*a.get()) < (*b.get());
}

bool operator==(const std::shared_ptr<Term>& a, const std::shared_ptr<Term>& b){
    return (*a.get()) == (*b.get());
}

Expression::Expression(){}

Expression::Expression(const Expression& other) { 
    terms = other.clone()->getTerms();
}

Expression& Expression::operator=(const Expression& other){
    terms = other.clone()->getTerms();

    return *this;
}

Expression::Expression(Expression&& other){
    std::swap(terms, other.terms);
}

bool Expression::equals(const Expression& exp) const{
    return terms == exp.getTerms();
}

std::string Expression::toString() const {
    std::string ret_val("");
    for (auto t : terms){
        ret_val+=t->toString()+" ";
    }
    
    if (terms.size() == 0)
        ret_val+="0";

    return ret_val;
}

}