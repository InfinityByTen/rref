// Matrix wrapper with basic serialization helper methods.
// Functions boost wrapper for uBlas Matrices.

#pragma once

#include <iostream>

#include <boost/numeric/ublas/matrix.hpp>

using Number = int;
using Matrix = boost::numeric::ublas::matrix<Number>;


static void
print( Matrix& mat )
{
    for (size_t i = 0; i < mat.size1( ); ++i)
    {
        for (size_t j = 0; j < mat.size2( ); ++j)
        {
            std::cout << mat(i, j)<<"\t";
        }
        std::cout << std::endl;
    }
    std::cout<<std::endl;
}


static void
input( Matrix& mat)
{
    for (size_t i = 0; i < mat.size1(); ++i)
    {
        for (size_t j = 0; j < mat.size2(); ++j)
        {
            Number n;
            std::cin>>n;
            mat.insert_element(i,j,n);
            std::cout<<std::endl;
        }
    }
}

