// An engine to compute the RREF of a Matrix.

#include "Matrix.h"

#include <boost/optional.hpp>

#include <chrono>
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

/* returns the index of the pivot column. If it is an all zero row, returns an overflow of the
 * column index.
 */
size_t
partial_pivot( Matrix& mat, size_t iteration )
{
    auto pivot_column = iteration;
    auto entry = get_max_abs_entry_in_column( mat, pivot_column, iteration );
    while ( entry.second == 0 && pivot_column < mat.columns( ) - 1 )
    {
        entry = get_max_abs_entry_in_column( mat, ++pivot_column, iteration );
    }

    if ( entry.second != 0 && entry.first != iteration )
    {
        // TODO: see if this can be any smarter.
        // swap max_index row with row corresponding to column_index
        for ( size_t column = 0; column < mat.columns( ); ++column )
        {
            Number temp;
            temp = mat( entry.first, column );
            mat( entry.first, column ) = mat( iteration, column );
            mat( iteration, column ) = temp;
        }
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
        // Crucial to keep factor a copy. auto casts it into reference.
        Number factor = mat( row, pivot_index ) / mat( iteration, pivot_index );

        for ( size_t column = 0; column < mat.columns( ); ++column )
        {
            mat( row, column ) -= mat( iteration, column ) * factor;
        }
    }
}

void
publish_metrics( std::chrono::duration< double > elapsed,
                 std::chrono::duration< double > pivoting,
                 std::chrono::duration< double > normalization,
                 std::chrono::duration< double > gauss_elimination )
{
    std::cout << "Total execution Time : " << elapsed.count( ) << std::endl;
    std::cout << "Pivoting: " << ( pivoting.count( ) / elapsed.count( ) ) * 100 << "/%"
              << std::endl;
    std::cout << "Normalization: " << ( normalization.count( ) / elapsed.count( ) ) * 100 << "/%"
              << std::endl;
    std::cout << "Gauss Elimination: " << ( gauss_elimination.count( ) / elapsed.count( ) ) * 100
              << "/%" << std::endl;
}

/*
 *computes the reduced row echelon form of a matrix.
 */
void
rref( Matrix& mat )
{
    auto start = std::chrono::high_resolution_clock::now( );
    std::chrono::duration< double > pivoting;
    std::chrono::duration< double > normalization;
    std::chrono::duration< double > gauss_elim;

    auto max_iter = std::min( mat.rows( ), mat.columns( ) );
    for ( size_t iteration = 0; iteration < max_iter; ++iteration )
    {
        auto tick = std::chrono::high_resolution_clock::now( );
        auto p = partial_pivot( mat, iteration );
        pivoting += ( std::chrono::high_resolution_clock::now( ) - tick );

        auto tick_norm = std::chrono::high_resolution_clock::now( );
        normalize( mat, iteration );
        normalization += ( std::chrono::high_resolution_clock::now( ) - tick_norm );

        if ( p == mat.columns( ) )
        {
            // all zero row.
            break;
        }

        auto tick_gs = std::chrono::high_resolution_clock::now( );
        gauss_elimination( mat, iteration, p );
        gauss_elim += ( std::chrono::high_resolution_clock::now( ) - tick_gs );
    }
    std::chrono::duration< double > elapsed
        = ( std::chrono::high_resolution_clock::now( ) - start );
    publish_metrics( elapsed, pivoting, normalization, gauss_elim );
}