#  Implementation for representing an instance of an optimization problem.

Some example code is available in the exampleCode.cpp file
To build it, run make in this directory, it will create a exempleCode executable

-----------------------------------------------------------

Here is the basic functionning of the representation :

This is an object based representation, meaning that each "element" of an optimization problem is an object.

The central class is the Model class. It holds the problem and its components.
It contains the variables and constraints, as well as objective functions.

Variables are represented via the Var class.

Now let's talk about the "big part" : the constraints. First, the Constraint class is an abstract base class for all constraints.
Each new constraint type must at least inherit from the Constraint class.
Then, there is the ExpressionConstraint class, that inherits from the Constraint class, and which is also a abstract class. It is meant to represent constraints that are made of an Expression.

Expressions are represented by the Expression base abstract class. Expressions are made of Term(s), which are composed of a variable and some other data.

Now let's take an exemple with linear constraints. First, we create the LinearConstr class. A linear constraint is made of an expression, so the LinearConstr class should inherit from ExpressionConstraint. 
Now, we have to specify the behavior of a linear expression. So we create a LinearExpr class that inherits from Expression. 
That requires us to define the structure of the terms of the linear expression. So we also create a LinearTerm class (or struct in the current implmenetation). 
A LinearTerm will be made of a coefficient (and as it inherits from Term it will also be made of a Var).

The Term struct requires that we define an "add" method that adds up 2 LinearTerm.
The Expression class requires us that we define an "addTerm" method, to add a new LinearTerm to the expression.



Beside the object based representation, there are the PackedVector and DCSRMatrix classes.
PackedVector is a utility class that uses a standard map to store an index/value pair.
DCSRMatrix is an implementation of the Dynamic-CSR matrix representation presented in this paper (https://thomas.gilray.org/pdf/dynamic-csr-journal.pdf)

There is the possibility to create LinearExpr with PackedVectors.
There is also the possibility to load a DCSRMatrix to a Model, and to export it from a Model to get a matrix with the coefficients of the linear constraints.


What to do next ?
- Add other constraint types
- Add new functionnalities to existing classes
- Figure out how to interface with the Plugin Manager
- Add support for this representation in the various solver shims