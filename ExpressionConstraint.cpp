#include "ExpressionConstraint.hpp"

namespace Osi2 {

ExpressionConstraint::ExpressionConstraint() {}

ExpressionConstraint::ExpressionConstraint(const std::string& name) : Constraint(name) {}

ExpressionConstraint::ExpressionConstraint(const ExpressionConstraint& other) : Constraint(other), expr(other.expr), bounds(other.bounds), format(other.format) {}

ExpressionConstraint::ExpressionConstraint(ExpressionConstraint&& other) {
    std::swap(expr, other.expr);
    std::swap(bounds, other.bounds);
    std::swap(format, other.format);
}

ExpressionConstraint& ExpressionConstraint::operator=(const ExpressionConstraint& other){
    Constraint::operator=(other);

    expr = other.expr;
    bounds = other.bounds;
    format = other.format;

    return *this;
}

std::string ExpressionConstraint::toString() const{
    std::string ret_val = "("+getName()+")     ";
    if ( getFormat() ==  ExpressionConstraint::Format::EQ ){
       ret_val+= expr->toString() + " = " + std::to_string(getBounds().lower_bound); 
    }
    else{
        auto lower_bound = getBounds().lower_bound == Range::NEGATIVE_INFINITY ? "-inf" : std::to_string(getBounds().lower_bound);
        auto upper_bound = getBounds().upper_bound == Range::POSITIVE_INFINITY ? "+inf" : std::to_string(getBounds().upper_bound);
        ret_val+= lower_bound + " <= " + expr->toString() + " <= " + upper_bound;
    }
    return ret_val;
}


}