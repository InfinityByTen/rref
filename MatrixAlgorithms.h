// An engine to compute the RREF of a Matrix.

#pragma once

#include "Matrix.h"
#include "ProfilingHelper.h"

#include <boost/optional.hpp>

#include <utility>

// TODO: Make the file into a class than free standing functions.

/* gets the maximum absolute entry in the given column starting from a row index.
 * returns pair(index, value)
 */
std::pair< size_t, Number >
get_max_abs_entry_in_column( Matrix& mat, size_t column_index, size_t start_row )
{
    size_t max_index = 0;
    Number max_entry = 0;
    for ( size_t row = start_row; row < mat.rows( ); ++row )
    {
        if ( abs( mat( row, column_index ) ) > max_entry )
        {
            max_entry = abs( mat( row, column_index ) );
            max_index = row;
        }
    }
    return std::make_pair( max_index, max_entry );
}

/* gets the next non-zero maximum absolute entry starting from a given pivot column.
 * returns pair(index, value)
 * pivot_column is an out-parameter.
 */
std::pair< size_t, Number >
find_next_pivot_entry( Matrix& mat, size_t iteration, size_t& pivot_column )
{
    auto pivot_entry = get_max_abs_entry_in_column( mat, pivot_column, iteration );
    while ( pivot_entry.second == 0 && pivot_column < mat.columns( ) - 1 )
    {
        pivot_entry = get_max_abs_entry_in_column( mat, ++pivot_column, iteration );
    }
    return pivot_entry;
}

void
swap_rows( Matrix& mat, size_t row1, size_t row2 )
{
    // TODO: see if this can be any smarter.
    for ( size_t column = 0; column < mat.columns( ); ++column )
    {
        // TODO: fix this
        // Crucial to keep factor a copy. auto casts it into reference.
        Number temp = mat( row1, column );
        mat( row1, column ) = mat( row2, column );
        mat( row2, column ) = temp;
    }
}

/* returns the index of the pivot column. If it is an all zero row, returns an overflow of the
 * column index.
 * Timekeepers are out-params for profiling
 */
size_t
partial_pivot( Matrix& mat,
               size_t iteration,
               Duration& max_abs_value_timekeeper,
               Duration& swapping_timekeeper )
{
    auto pivot_column = iteration;

    auto entry = execute_with_profiling( find_next_pivot_entry, max_abs_value_timekeeper, mat,
                                         (size_t)iteration, (size_t&)pivot_column );

    if ( entry.second != 0 && entry.first != iteration )
    {
        // swap max_index row with row corresponding to column_index
        execute_with_profiling( swap_rows, swapping_timekeeper, mat, (size_t)entry.first,
                                (size_t)iteration );
        return pivot_column;
    }
    if ( entry.second == 0 )
    {
        // TODO: Fix hacky solution for complete zero row
        return pivot_column + 1;
    }
    return pivot_column;
}

/* normalizes the row corresponding to the row_index (zero based)
 * with respect to the first non-zero element.
 */
void
normalize( Matrix& mat, size_t row_index )
{
    // TODO: preferably lambda-fy
    boost::optional< Number > non_zero;
    for ( size_t column = 0; column < mat.columns( ); ++column )
    {
        if ( mat( row_index, column ) == 0 )
        {
            continue;
        }
        if ( non_zero )
        {
            mat( row_index, column ) /= non_zero.get( );  // cannot throw exception. Always defined.
        }
        else
        {
            non_zero = mat( row_index, column );
            mat( row_index, column ) = 1;
        }
    }
}

/* performs one iteration of (full) gauss elimination.
 * Reduces all the rows based on the pivot_index column in the row of the iteration value.
 * (iteration, pivot) is the next largest element in absolute values, that can be used for the row
 * reduction of the matrix. This is a modified form that's needed for reduced row echelon form
 * instead of the one that converts it into an upper triangular one.
 */

void
gauss_elimination( Matrix& mat, size_t iteration, size_t pivot_index )
{
    for ( size_t row = 0; row < mat.rows( ); ++row )
    {
        if ( row == iteration )
        {
            continue;
        }
        // TODO: Fix this
        // Crucial to keep factor a copy. auto casts it into reference.
        Number factor = mat( row, pivot_index ) / mat( iteration, pivot_index );

        for ( size_t column = 0; column < mat.columns( ); ++column )
        {
            mat( row, column ) -= mat( iteration, column ) * factor;
        }
    }
}

/*
 *computes the reduced row echelon form of a matrix.
 */
void
rref( Matrix& mat )
{
    auto start = std::chrono::high_resolution_clock::now( );
    Duration pivoting_timekeeper;
    Duration max_abs_value_timekeeper;
    Duration swapping_timekeeper;
    Duration normalization_timekeeper;
    Duration gauss_elim_timekeeper;

    auto max_iter = std::min( mat.rows( ), mat.columns( ) );
    for ( size_t iteration = 0; iteration < max_iter; ++iteration )
    {
        auto pivot = execute_with_profiling( partial_pivot, pivoting_timekeeper, mat,
                                             (size_t)iteration, (Duration&)max_abs_value_timekeeper,
                                             (Duration&)swapping_timekeeper );

        execute_with_profiling( normalize, normalization_timekeeper, mat, (size_t)iteration );

        if ( pivot == mat.columns( ) )
        {
            // all zero row.
            break;
        }
        execute_with_profiling( gauss_elimination, gauss_elim_timekeeper, mat, (size_t)iteration,
                                (size_t)pivot );
    }

    Duration elapsed = ( std::chrono::high_resolution_clock::now( ) - start );
    publish_metrics( elapsed, pivoting_timekeeper, max_abs_value_timekeeper, swapping_timekeeper,
                     normalization_timekeeper, gauss_elim_timekeeper );
}