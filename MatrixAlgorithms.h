// An engine to compute the RREF of a Matrix.

#include "Matrix.h"

void
gauss_elimination(Matrix& mat)
{
    for (int iteration = 1; iteration < mat.size1(); ++iteration)
    {
        for (int row = iteration; row < mat.size1(); ++row)
        {
            Number factor = mat(row,iteration-1)/mat(iteration-1,iteration-1);
            for (int column = 0; column < mat.size2(); ++column)
            {
                mat(row,column) -= mat(iteration-1,column) * factor;
            }
        }
    }
}