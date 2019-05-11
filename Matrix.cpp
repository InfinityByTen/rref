// Implementation of Matrix Wrapper

#include "Matrix.h"

Matrix::Matrix(size_t rows, size_t columns)
: boost::numeric::ublas::matrix<Number>(rows, columns)
{
}

void
Matrix::print( ) const
{
    for (size_t row = 0; row < size1( ); ++row)
    {
        for (size_t column = 0; column < size2( ); ++column)
        {
            std::cout << operator()(row, column)<<"\t";
        }
        std::cout << std::endl;
    }
    std::cout<<std::endl;
}

void
Matrix::input( )
{
    for (size_t row = 0; row < size1(); ++row)
    {
        for (size_t column = 0; column < size2(); ++column)
        {
            Number n;
            std::cin>>n;
            insert_element(row,column,n);
        }
    }
}