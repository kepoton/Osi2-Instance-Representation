#include "QuadraticConstraint.hpp"

namespace Osi2 {

QuadraticConstraint::QuadraticConstraint() {}

QuadraticConstraint::QuadraticConstraint(const QuadraticExpr& expr, const Range& bounds) {
    setExpr(expr);
    setBounds(bounds);
    if ( bounds.lower_bound == bounds.upper_bound ){
         setFormat(ExpressionConstraint::Format::LE);
    }
    else {
         setFormat(ExpressionConstraint::Format::EQ);
    }
}

QuadraticConstraint::QuadraticConstraint(const QuadraticExpr& expr) {
    setExpr(expr);
    setFormat(ExpressionConstraint::Format::LE);
}

QuadraticConstraint::QuadraticConstraint(const QuadraticConstraint& other) : ExpressionConstraint(other) {
    setExpr(other.getExpr());
    setBounds(other.getBounds());
    setFormat(other.getFormat());
}

QuadraticConstraint::QuadraticConstraint(QuadraticConstraint&& other) {
    std::swap(expr, other.expr);
    setBounds(other.getBounds());
    setFormat(other.getFormat());
}

QuadraticConstraint& QuadraticConstraint::operator=(const QuadraticConstraint& other){
    expr = other.expr;
    setBounds(other.getBounds());
    setFormat(other.getFormat());

    return (*this);
}

std::ostream& operator<<(std::ostream& flux, const QuadraticConstraint& constr){
    std::cout << "(" << constr.getName() << ")     "; 
    const QuadraticExpr* e = dynamic_cast<const QuadraticExpr*>(&constr.getExpr()); 
    if ( constr.getFormat() ==  ExpressionConstraint::Format::EQ ){
       flux << *e << " = " << constr.getBounds().lower_bound; 
    }
    else{
        flux << constr.getBounds().lower_bound << " <= " << *e << " <= " << constr.getBounds().upper_bound;
    }

    return flux;
}

QuadraticConstraint operator<=(const QuadraticConstraint& c, double up){
    QuadraticConstraint ret_val(c);

    ret_val.setUpperBound(up);
    if ( ret_val.getBounds().lower_bound == up ){
        ret_val.setFormat(ExpressionConstraint::Format::EQ);
    }
    else {
       ret_val.setFormat(ExpressionConstraint::Format::LE); 
    }

    return ret_val;
}

QuadraticConstraint operator<=( double low, const QuadraticConstraint& c){
    QuadraticConstraint ret_val(c);

    ret_val.setLowerBound(low);
    if ( ret_val.getBounds().upper_bound == low ){
        ret_val.setFormat(ExpressionConstraint::Format::EQ);
    }
    else {
       ret_val.setFormat(ExpressionConstraint::Format::LE); 
    }

    return ret_val;
}

QuadraticConstraint operator==(const QuadraticExpr& exp1, double n){
    QuadraticConstraint ret_val;

    ret_val.setExpr(exp1);
    ret_val.setBounds(Range(n));
    ret_val.setFormat(ExpressionConstraint::Format::EQ);

    return ret_val;
}

}