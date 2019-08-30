#include "DCSRMatrix.hpp"
#include "Model.hpp"

using namespace Osi2;

void exampleDCSRMatrix(){
    DCSRMatrix matrix1;
    DCSRMatrix matrix2;
    PackedVector v;

    for (uint32_t i = 0; i < 10; i++){ 
        for (uint32_t j = 0; j < 10; j++){ // Put random value in the vector
            v.insert(j, i);
        }
        matrix1.addRow(v); // Add the vector as a row in  matrix1
        matrix2.addColumn(v); // Add the vector as a column in  matrix2
        v.clear(); // Clear the content of the vector
    }

    matrix1.display();

    matrix2.display();
}

void exampleModelBuilding(){
    Model m; 
    uint32_t v1 = m.addVariable("A", Range(0, 5), Var::Domaine::REAL); // Add a variable to the model and return its id
    uint32_t v2 = m.addVariable(Range(0, 10), Var::Domaine::REAL);
    m[v2].addRange(Range(2, 14)); // operator[] overload for the Model object. m[v2] is equivalent to m.getVariable(v2)
    uint32_t v3 = m.addVariable("C", Range(0, 15), Var::Domaine::REAL);

    LinearExpr e1 = m[v1]; // Building linear expressions with operator overload
    LinearExpr e2 = 3 * m[v2] - m[v3];
    LinearExpr e3 = m[v1] - 2 * m[v2];
    e3.addTerm(-4, m[v1]); // Building linear expressions by adding terms one by one
    
    QuadraticExpr qe1 = QuadraticTerm(1,2,3,m[v1]); // Building quadratic expressions with operator overload
    qe1 += QuadraticTerm(1,2,3,m[v1]) - QuadraticTerm(4,0,8,m[v3]);
    qe1.addTerm( 1, 2, 3 , m[v1] ); // Building quadratic expressions by adding terms one by one
    qe1.addTerm( 4, 0, 8 , m[v3] );

    LinearConstr c1 = 3 <= e1 <= 8; // Building a linear constraint with operator overload
    LinearConstr c2 = 0 <= e1 - e2 <= 4;
    LinearConstr c3 = e3;
    c3.setBounds(Range(-5, 2));

    QuadraticConstraint qc1 = 15 <= qe1 <= 65; // Building a quadratic constraint with operator overload
    QuadraticConstraint qc2 = qe1 - qe1 == 2;
    
    m.addConstraint(c1, "c1"); // Add a constraint to the model and give it a name
    m.addConstraint(c2, "c2");
    m.addConstraint(c3, "c3");

    m.addConstraint(qc1, "qc1");
    m.addConstraint(qc2, "qc2");
    
    m.addObjectiveFun("Objective1", e2, Objective::Type::MINIMIZE); // Add an objective function to the model
    m.addObjectiveFun("Objective2", e3, Objective::Type::MINIMIZE); // Add an other objective function to the model

    m.display(); // Display the model on standard output. Mainly, for debugging purpose
}

void exampleModelBuildingWithMatrix(){
    DCSRMatrix matrix;
    PackedVector v;
    std::vector<double> lower_bounds;
    std::vector<double> upper_bounds;

    for (uint32_t i = 0; i < 10; i++){
        for (uint32_t j = 0; j < 10; j++){ // Put random value in the vector
            v.insert(j, i);
        }
        matrix.addRow(v); // Add the vector as a row in the matrix
        lower_bounds.push_back(i); // Set a random lower bound for the constraint
        upper_bounds.push_back(i+5); // Set a random upper bound for the constraint
        v.clear();
    }

    Model m;
    m.fromMatrix(matrix, lower_bounds, upper_bounds); // Import the matrix and bound vectors to the model

    PackedVector vec;
    vec.insert(0, 6);
    vec.insert(5, 2);
    vec.insert(2, 9);

    LinearExpr exp(vec, m); // Create a random expression

    m.addObjectiveFun("Obj", exp, Objective::Type::MINIMIZE); // Add expression as objective function

    m.display();
}

int main(){
    exampleDCSRMatrix();
    exampleModelBuilding();
    exampleModelBuildingWithMatrix();

    return 0;
}