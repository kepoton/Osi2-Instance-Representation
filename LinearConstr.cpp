#include "LinearConstr.hpp"
#include "Model.hpp"

#include <exception>

namespace Osi2 {

LinearConstr::LinearConstr() {}

LinearConstr::LinearConstr(const LinearExpr& expr, const Range& bounds) {
    setExpr(expr);
    setBounds(bounds);
    if ( bounds.lower_bound == bounds.upper_bound ){
         setFormat(ExpressionConstraint::Format::LE);
    }
    else {
         setFormat(ExpressionConstraint::Format::EQ);
    }
}

LinearConstr::LinearConstr(const LinearExpr& expr) {
    setExpr(expr);
    setFormat(ExpressionConstraint::Format::LE);
}

LinearConstr::LinearConstr(const LinearConstr& other) : ExpressionConstraint(other) {
    setExpr(other.getExpr());
    setBounds(other.getBounds());
    setFormat(other.getFormat());
}

LinearConstr::LinearConstr(LinearConstr&& other) {
    std::swap(expr, other.expr);
    setBounds(other.getBounds());
    setFormat(other.getFormat());
}

LinearConstr::LinearConstr(const PackedVector& coefs, const Range& bounds, Model& m) {
    LinearExpr ex(coefs, m);

    setExpr(ex);
    setBounds(bounds);
    if (bounds.lower_bound == bounds.upper_bound){
        setFormat(ExpressionConstraint::Format::EQ);
    }
    else{
        setFormat(ExpressionConstraint::Format::LE);
    }
}

LinearConstr& LinearConstr::operator=(const LinearConstr& other){
    expr = other.expr;
    setBounds(other.getBounds());
    setFormat(other.getFormat());

    return (*this);
}

std::ostream& operator<<(std::ostream& flux, const LinearConstr& constr){
    std::cout << "(" << constr.getName() << ")     "; 
    const LinearExpr* e = dynamic_cast<const LinearExpr*>(&constr.getExpr()); 
    if ( constr.getFormat() ==  ExpressionConstraint::Format::EQ ){
       flux << *e << " = " << constr.getBounds().lower_bound; 
    }
    else{
        flux << constr.getBounds().lower_bound << " <= " << *e << " <= " << constr.getBounds().upper_bound;
    }

    return flux;
}

LinearConstr operator<=(const LinearConstr& c, double up){
    LinearConstr ret_val(c);

    ret_val.setUpperBound(up);
    if ( ret_val.getBounds().lower_bound == up ){
        ret_val.setFormat(ExpressionConstraint::Format::EQ);
    }
    else {
       ret_val.setFormat(ExpressionConstraint::Format::LE); 
    }

    return ret_val;
}

LinearConstr operator<=( double low, const LinearConstr& c){
    LinearConstr ret_val(c);

    ret_val.setLowerBound(low);
    if ( ret_val.getBounds().upper_bound == low ){
        ret_val.setFormat(ExpressionConstraint::Format::EQ);
    }
    else {
       ret_val.setFormat(ExpressionConstraint::Format::LE); 
    }

    return ret_val;
}

LinearConstr operator==(const LinearConstr& exp1, double n){
    LinearConstr ret_val;

    ret_val.setExpr(exp1.getExpr());
    ret_val.setBounds(Range(n));
    ret_val.setFormat(ExpressionConstraint::Format::EQ);

    return ret_val;
}

}