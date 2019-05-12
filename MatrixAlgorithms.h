// An engine to compute the RREF of a Matrix.

#include "Matrix.h"

bool
gauss_elimination(Matrix& mat)
{
    for (int iteration = 1; iteration < mat.rows( ); ++iteration)
    {
        for (int row = iteration; row < mat.rows( ); ++row)
        {
            Number factor;
            try
            {
                factor = mat(row,iteration-1)/mat(iteration-1,iteration-1);
            }
            catch(const std::overflow_error)
            {
                return false;
            }
            for (int column = 0; column < mat.columns( ); ++column)
            {
                mat(row,column) -= mat(iteration-1,column) * factor;
            }
        }
    }
    return true;
}