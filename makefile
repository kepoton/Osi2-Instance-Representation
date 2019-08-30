
INCLUDE_PATH=/mnt/d/Kevin/Bureau/OSI_/Osi2/trunk/build/include
LIB_PATH=/mnt/d/Kevin/Bureau/OSI_/Osi2/trunk/build/lib
INCLUDE_PATH_CPX=/opt/cplex/CPLEX_Studio129/cplex/include/ilcplex
LIB_PATH_CPX=/opt/cplex/CPLEX_Studio129/cplex/lib/x86-64_linux/static_pic
INCLUDE_PATH_GRB=/opt/gurobi811/linux64/include
LIB_PATH_GRB=/opt/gurobi811/linux64/lib
LIBS=-lOsiClp -lClp -lOsi -lcoinglpk -ldl -lm -lCoinUtils -lOsiCpx -lcplex
FLAGS=-g -Wall -O3 -std=c++11 -pedantic -Wextra

CC=g++

all: PackedVector.cpp DCSRMatrix.cpp Model.cpp Range.cpp Var.cpp LinearExpr.cpp LinearConstr.cpp QuadraticExpr.cpp QuadraticConstraint.cpp ExpressionConstraint.cpp Constraint.cpp Expression.cpp
	${CC} ${FLAGS} -o exampleCode exampleCode.cpp $^

PackedVector.cpp : PackedVector.hpp

DCSRMatrix.cpp : DCSRMatrix.hpp PackedVector.cpp

Model.cpp : Model.hpp

Range.cpp : Range.hpp

Var.cpp : Var.hpp

Expression.cpp : Expression.hpp

LinearExpr.cpp : LinearExpr.hpp

QuadraticExpr.cpp : QuadraticExpr.hpp

Constraint.cpp : Constraint.hpp

ExpressionConstraint.cpp : ExpressionConstraint.hpp

LinearConstr.cpp : LinearConstr.hpp

QuadraticConstraint.cpp : QuadraticConstraint.hpp





clean:
	rm *.o
