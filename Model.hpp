#ifndef _MODEL_HPP
#define _MODEL_HPP

#include "DCSRMatrix.hpp"

#include "ExpressionConstraint.hpp"
#include "LinearConstr.hpp"
#include "QuadraticConstraint.hpp"

#include <set>
#include <unordered_map>
#include <vector>
#include <memory>

namespace Osi2 {

/*! \brief Helper for the matrix representation of linear constraint

    Used for importing/exporting matrix to/from the Model
*/

struct MatrixHelper {
    DCSRMatrix matrix;
    std::vector<double> lower_bounds;
    std::vector<double> upper_bounds;
};

/*! \brief Representation of an objective function
    
    An objective function is composed of an expression and a type (MINIMIZE or MAXIMIZE)
 */
struct Objective {
    /// Type of objective function
    enum class Type { 
        MAXIMIZE, ///< Maximize the objective function 
        MINIMIZE  ///< Minimize the objective function
    };
    std::shared_ptr<Expression> expr;
    //LinearExpr expr; ///< Expression of the objective function
    Type type; ///< Type (MINIMIZE or MAXIMIZE) of the objective function

    // Constructor
    Objective(const Expression& exp, Objective::Type t) : type(t) { 
        switch (exp.getType() ){
            case Expression::Type::LINEAR:
                expr = std::make_shared<LinearExpr>(exp);
                break;
            case Expression::Type::QUADRATIC:
                expr = std::make_shared<QuadraticExpr>(exp);
                break;
        }
    }
};

/*! \brief Main class for representing the model of a problem

    The purpose of this class is to centralize all the data related to a problem
 */
class Model {

    public:

        /// Enum to represent the type of a Model
        enum class Type {
            LINEAR,
            QUADRATIC,
            GENERAL
        };

        /// \name Constructors
        //{@
        
        /// Default constructor
        Model();
        //@}

        /// \name Editing functions
        //{@

        /// Add a variable to the model. Returns its id
        uint32_t addVariable(Var::Domaine d = Var::Domaine::REAL);

        /// Add a variable to the model and give it a Range. Returns its id
        uint32_t addVariable(const Range& range, Var::Domaine d = Var::Domaine::REAL);

        /// Add a variable to the model and give it a name. Returns its id
        uint32_t addVariable(const std::string& name, Var::Domaine d = Var::Domaine::REAL);

        /// Add a variable to the model, give it a name and a Range. Returns its id
        uint32_t addVariable(const std::string& name, const Range& range, Var::Domaine d = Var::Domaine::REAL);

        /// Add a given constraint to the model, and give it a name. Returns its id
        uint32_t addConstraint(const ExpressionConstraint& constraint, const std::string& name = ""); //TODO : check names before registering the constraint

        /// Add a constraint created using a PackedVector of coefficients and a Range, and give it a name
        uint32_t addConstraint(const PackedVector& constraints_coef, const Range& range, const std::string& name = "");

        /// Remove the constraint designated by the index
        bool removeConstraint(uint32_t index);

        /// Remove the constraint designated by the name
        bool removeConstraint(const std::string& name);

        /// Add an objective function to the model
        bool addObjectiveFun(const std::string& name, const LinearExpr& exp, Objective::Type type) { return objectives.insert( std::make_pair( name, Objective(exp, type) )).second; }
        
        /// Remove the objective function designated by the name
        bool removeObjectiveFun(const std::string& name) { return objectives.erase(name) == 1; } //TODO check if obj functions uses variables actually contained in the model

        //@}

        /// \name Getters
        //{@

        /// Get the objective function designated by the name
        const Objective& getObjectiveFun(const std::string& name) const { return objectives.at(name); }

        /// Get a Var via its id
        Var& getVariable(uint32_t id);

        /// Get a Var via its name
        Var& getVariable(const std::string& name);

        /// Get the index of a variable in the vector
        int getVariableIndex(const Var& v) const;

        /// Get the variable at a given index (column)
        Var& getVariableAtIndex(uint32_t index);

        /// Get a Var via its id with operator overload
        Var& operator[](uint32_t id);

        /// Get a Var via its name with operator overload
        Var& operator[](const std::string& name);

        /// Get the begin iterator from the vector of Var
        std::vector<Var>::const_iterator varsIteratorBegin() const { return std::begin(vars); };

        /// Get the end iterator from the vector of Var
        std::vector<Var>::const_iterator varsIteratorEnd() const { return std::end(vars); };

        /// Get a Constraint via its id
        Constraint& getConstraint(uint32_t id);

        /// Get a Constraint via its name
        Constraint& getConstraint(const std::string& name);

        /// Get a Constraint via its id with operator overload
        Constraint& operator()(uint32_t id);

        /// Get a Constraint via its name with operator overload
        Constraint& operator()(const std::string& name);

        /// Get the type of the model
        Model::Type getType() const;

        /// Get the begin iterator from the vector of Var
        std::vector<std::shared_ptr<Constraint>>::const_iterator constraintsIteratorBegin() const { return std::begin(constraints); } ;

        /// Get the end iterator from the vector of Var
        std::vector<std::shared_ptr<Constraint>>::const_iterator constraintsIteratorEnd() const { return std::end(constraints); } ;
        //@}
        
        /// Export the constraint's coefficients as a DCSRMatrix, in the case of a linear problem, using the MatrixHelper
        MatrixHelper toMatrix() const;

        /// Import a matrix of coefficient as constraints in the model using the MatrixHelper
        void fromMatrix(const MatrixHelper& matrix);

        /// Import a matrix of coefficient as constraints in the model
        void fromMatrix(const DCSRMatrix& matrix, const std::vector<double>& lower_bounds, const std::vector<double>& upper_bounds);

        /// For debug purpose only
        void display();

    private:
        std::unordered_map<std::string, Objective> objectives; ///< Map of the objective functions
        std::vector<Var> vars; ///< Vector of the variables in the problem
        std::vector<std::shared_ptr<Constraint>> constraints; ///< Vector of constraints
};

}

#endif //_MODEL_HPP