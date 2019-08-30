#include "Model.hpp"

#include "LinearConstr.hpp"
#include "QuadraticConstraint.hpp"

#include <iostream>

namespace Osi2 {

Model::Model(){}

uint32_t Model::addVariable(Var::Domaine d){
    return addVariable(Range(), d);
}

uint32_t Model::addVariable(const Range& range, Var::Domaine d){
    
    vars.emplace_back(range, d);
    return vars.back().getID();
}

uint32_t Model::addVariable(const std::string& name, Var::Domaine d){
    return addVariable(name, Range(), d);
}

uint32_t Model::addVariable(const std::string& name, const Range& range, Var::Domaine d){
    vars.emplace_back(name, range, d);
    return vars.back().getID();
}


uint32_t Model::addConstraint(const ExpressionConstraint& constraint, const std::string& name){
    bool success = true;
    std::shared_ptr<Constraint> c;

    //Check if all the variables of the constraint exist in the model
    auto expr_it = std::begin(constraint);
    while ( success && expr_it != std::end(constraint) ){ // For each term in the expression of the constraint
        bool found = false;
        auto vars_it = std::begin(vars);
        while (!found && vars_it != std::end(vars)){ // Compare it with the variables stored in the model
            if ((*vars_it) == expr_it->get()->var) {
                found = true; // We found the variable in the model
            }
            ++vars_it;
        }
        if (!found){
            success = false; // The variable does not exist in the model
        }

        ++expr_it;
    }

    if (success){ 
        switch (constraint.getType()){ // Allocate the new constraint to put in the model, based on its type. Copy constructed from the constraint passed as argument
            case Constraint::Type::LINEAR:
                c = std::make_shared<LinearConstr>(constraint);
                break;
            case Constraint::Type::QUADRATIC:
                c = std::make_shared<QuadraticConstraint>(constraint);
                break;
            default:
                // TODO : Add cases here as you add constraints type
                break;
        }

        if (name != ""){ // If not default name parameter
            c->setName(name);
        }
        constraints.push_back(c);
    }

    if (success)
        return c->getID();
    else
        return 0;
}

uint32_t Model::addConstraint(const PackedVector& constraints_coef, const Range& range, const std::string& name){
    if (constraints_coef.size() != 0){
        for (uint32_t i = vars.size(); i <= constraints_coef.maxIndex(); i++){
            addVariable();
        }
    }

    return addConstraint(LinearConstr(constraints_coef, range, *this), name);
}

bool Model::removeConstraint(uint32_t index){
    try{
        constraints.at(index);
    }catch(std::out_of_range e){
        throw std::out_of_range("No constraint at index " + std::to_string(index) + ". Max index is " + std::to_string(constraints.size() -1) );
    }

    constraints.erase(std::begin(constraints) + index );

    return true;
}

bool Model::removeConstraint(const std::string& name){
    bool ret_val = false;

    auto it = std::begin(constraints);
    while (it != std::end(constraints) && it->get()->getName() != name ) ++it; // Look for the constraint with the given name

    if (it != std::end(constraints)){ // If the constraint was found
        constraints.erase(it); // Delete it
        ret_val = true;
    }

    return ret_val;
}

Var& Model::getVariable(uint32_t id){
    auto it = std::begin(vars);
    bool found = false;
    while (!found && it != std::end(vars)) // Search for the variable with the given id
    {
        if (it->getID() == id){
            found = true;
        }
        else{
            ++it;
        }
    }

    if (!found){
        throw std::invalid_argument("Not variable with id "+std::to_string(id)+" in this model");
    }
    return *it;
}

Var& Model::getVariable(const std::string& name){
    auto it = std::begin(vars);
    bool found = false;
    while (!found && it != std::end(vars)) // Search for the variable with a given name
    {
        if (it->getName() == name){
            found = true;
        }
        else{
            ++it;
        }
    }

    if (!found){
        throw std::invalid_argument("Not variable with name "+name+" in this model");
    }
    return *it;
}

int Model::getVariableIndex(const Var& var) const {
    int index = 0;
    bool found = false;
    auto it = std::begin(vars);
    while ( !found && it != std::end(vars) ){
        if ( equals(*it, var) ){
            found = true;
        }
        else{
            ++index;
        }
        ++it;
    }

    if (found)
        return index;
    else
        return -1;
}

Var& Model::getVariableAtIndex(uint32_t index){
    if (index >= vars.size()){
        throw std::invalid_argument("Index out of bound, not variable at index " + index);
    }
    return vars[index];   
}

Var& Model::operator[](uint32_t id){
    return getVariable(id);
}

Var& Model::operator[](const std::string& name){
    return getVariable(name);
}

Constraint& Model::getConstraint(uint32_t id){
    auto it = std::begin(constraints);
    bool found = false;
    while (!found && it != std::end(constraints)) // Look for the constraint with a given id
    {
        if (it->get()->getID() == id){
            found = true;
        }
        else{
            ++it;
        }
    }

    if (!found){
        throw std::invalid_argument("Not constraint with id "+std::to_string(id)+" in this model");
    }
    return *(it->get());
}

Constraint& Model::getConstraint(const std::string& name){
    auto it = std::begin(constraints);
    bool found = false;
    while (!found && it != std::end(constraints)) // Look for the constraint with a given name
    {
        if (it->get()->getName() == name){
            found = true;
        }
        else{
            ++it;
        }
    }

    if (!found){
        throw std::invalid_argument("Not constraint with id "+name+" in this model");
    }
    return *(it->get());
}

Constraint& Model::operator()(uint32_t id){
    return getConstraint(id);
}

Constraint& Model::operator()(const std::string& name){
    return getConstraint(name);
}

Model::Type Model::getType() const {
    Model::Type ret_val = Model::Type::LINEAR;
    uint32_t val;

    for ( auto c : constraints ){ // Iterate the constraints to get the type of the model
        switch (c->getType()){
            case Constraint::Type::LINEAR:
                val = 0;
                break;
            case Constraint::Type::QUADRATIC:
                val = 1;
                break;
            default:
                val = 2;
                break;
        }

        if (val > (uint32_t)ret_val)
            ret_val = (Model::Type)val;
    }

    return ret_val;
}

MatrixHelper Model::toMatrix() const{
    MatrixHelper ret_val;    
    DCSRMatrix matrix(0, vars.size());
    std::vector<double> lower_b;
    std::vector<double> upper_b;

    PackedVector v;
    for (const auto& c : constraints){ // For each constraint
        if (c->getType() == Constraint::Type::LINEAR){ // If it is linear
            ExpressionConstraint* exp_c = static_cast<ExpressionConstraint*>(c.get());
            for (const auto& e : exp_c->getExpr().getTerms()){ // Add each of its coefficients to a PackedVector
                LinearTerm* term = static_cast<LinearTerm*>(e.get());
                uint32_t v_ind = getVariableIndex(e->var); // Get to which column a variable belongs
                v.insert( v_ind, term->coef );
            }
            matrix.addRow(v); // Then add the PackedVector as a new row in the matrix
            lower_b.push_back(exp_c->getLowerBound());
            upper_b.push_back(exp_c->getUpperBound());
            v.clear();
        }
    }

    ret_val.matrix = matrix;
    ret_val.lower_bounds = lower_b;
    ret_val.upper_bounds = upper_b;

    return ret_val;
}

void Model::fromMatrix(const MatrixHelper& helper){
    try{
        for (uint32_t i = 0; i < helper.matrix.getRowCount(); i++){
            addConstraint(helper.matrix.getRow(i), Range(helper.lower_bounds[i], helper.upper_bounds[i]));
        }
    }
    catch(const std::exception& e){
        std::cerr << "Error while importing matrix coefficients as constraints : \n" << e.what();
    }
}

void Model::fromMatrix(const DCSRMatrix& matrix, const std::vector<double>& lower_bounds, const std::vector<double>& upper_bounds){
    MatrixHelper helper = { matrix, lower_bounds, upper_bounds };
    fromMatrix(helper);
}

void Model::display(){
    std::cout << "Vars : " ;
    for ( const auto& v : vars){
        std::cout << v.getName() << " ";
    }

    std::cout << "\n\nObjectives : \n" ;
    for ( const auto& obj : objectives){
        std::cout << "(" << obj.first << ")";
        if ( obj.second.type == Objective::Type::MINIMIZE ){
            std::cout << "    MINIMIZE  ";
        }
        else{
           std::cout << "    MAXIMIZE  "; 
        }

        std::cout << obj.second.expr->toString() << "\n";
    }

    std::cout << "\n" << std::endl;

    for ( const auto& c : constraints ){
        std::cout << c->toString() << std::endl;
    }

    std::cout << "\n\nWith : " << std::endl;

    for ( const auto& var : vars ){
        for (const auto& range : var.getRanges()){
            std::cout << "            " << range.lower_bound << " <= " << var.getName() << " <= " << range.upper_bound << std::endl;
        }
        
    }
}

}

