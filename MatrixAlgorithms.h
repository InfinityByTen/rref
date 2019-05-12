// An engine to compute the RREF of a Matrix.

#include "Matrix.h"

#include <boost/optional.hpp>

Number
partial_pivot(Matrix& mat, size_t column_index)
{
    size_t max_index = 0;
    Number max_entry = 0;
    for (size_t row = column_index; row < mat.rows(); ++row)
    {
        if(abs(mat(row,column_index))> max_entry)
        {
            max_entry = mat(row, column_index);
            max_index = row;
        }
    }

    if(max_index!=column_index && max_entry!=0)
    {
        // TODO: see if this can be any smarter.
        // swap max_index row with row corresponding to column_index
        for (size_t column = 0; column < mat.columns( ); ++column)
        {
            Number temp;
            temp = mat(max_index,column);
            mat(max_index,column) = mat(column_index, column);
            mat(column_index, column) = temp;
        }
    }
    return max_entry;
}

/* normalizes the row corresponding to the row_index (zero based)
 * with respect to the first non-zero element.
 */
void
normalize(Matrix& mat, size_t row_index)
{
    // TODO: preferably lambda-fy
    boost::optional<Number> non_zero;
    for (size_t column = 0; column < mat.columns( ); ++column)
    {
        if(mat(row_index, column) == 0)
        {
            continue;
        }
        if(non_zero)
        {
            mat(row_index,column) /= non_zero.get();
        }
        else
        {
            non_zero = mat(row_index, column);
            mat(row_index,column) = 1;
        }
    }
}

bool
rref(Matrix& mat)
{
    for (size_t iteration = 0; iteration < mat.rows( ); ++iteration)
    {
        Number p = partial_pivot(mat, iteration );
        if( p !=0 )
        {
            normalize(mat, iteration);
        }

        // TODO: extract out below as gauss elimination step
        for (size_t row = 0 ; row < mat.rows( ); ++row)
        {
            if(row == iteration || p == 0)
            {
                continue;
            }
            Number factor;
            try
            {
                factor = mat(row,iteration)/mat(iteration,iteration);
            }
            catch(const std::overflow_error)
            {
                return false;
            }
            for (size_t column = 0; column < mat.columns( ); ++column)
            {
                mat(row,column) -= mat(iteration,column) * factor;
            }
        }
    }
    return true;
}