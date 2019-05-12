// Matrix wrapper with basic serialization helper methods.

#pragma once

#include <boost/multiprecision/gmp.hpp>
#include <boost/numeric/ublas/matrix.hpp>

using Number = boost::multiprecision::mpq_rational;

class Matrix
{
public:
    // Initializes an empty matrix of size rows x columns
    Matrix(size_t rows, size_t columns);

    // Initializes a matrix with entries in the vector
    Matrix(size_t rows, size_t columns, const std::vector<Number>& entries);

    // Matrix Sizes
    size_t rows( ) const;
    size_t columns( ) const;

    // Reference to an element
    Number& operator()(size_t row, size_t column);
    const Number& operator()(size_t row, size_t column) const;

    bool operator==(const Matrix& other) const;

    // Pretty prints out the full matrix.
    void print( ) const;

    // Inputs the matrix entry by entry row-wise
    void input( );
private:
    boost::numeric::ublas::matrix<Number> m_impl;
};