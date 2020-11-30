#include "RMatrix.h"
#include <iostream>

int main()
{
    freopen("E:\\C&C++\\in.txt", "r", stdin);
    freopen("E:\\C&C++\\out.txt", "w", stdout);
    int n;
    std::cin >> n;
    RMatrix rmatrix = RMatrix(n, REFLEXIVE | ANTISYMMETRIC | TRANSITIVE, 5);
    rmatrix.printRMatrix();
    rmatrix.printAttrs();

    // RMatrix rmatrix = RMatrix(n, REFLEXIVE | ANTISYMMETRIC | TRANSITIVE, 30);
    // rmatrix.printRMatrix();
    // rmatrix.printAttrs();
    return 0;
}