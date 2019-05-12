// Matrix wrapper with basic serialization helper methods.
// Inherits from boost wrapper for uBlas Matrices.

#pragma once

#include <boost/multiprecision/gmp.hpp>
#include <boost/numeric/ublas/matrix.hpp>

using Number = boost::multiprecision::mpq_rational;

class Matrix : public boost::numeric::ublas::matrix<Number>
{
public:
    // Initializes an empty matrix of size rows x columns
    Matrix(size_t rows, size_t columns);

    // Pretty prints out the full matrix.
    void print( ) const;

    // Inputs the matrix entry by entry row-wise
    void input( );
};