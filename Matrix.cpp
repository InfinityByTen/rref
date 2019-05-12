// Implementation of Matrix Wrapper

#include "Matrix.h"

#include <iostream>

Matrix::Matrix(size_t rows, size_t columns)
: m_impl( boost::numeric::ublas::matrix<Number>(rows, columns))
{
}

Matrix::Matrix(size_t rows, size_t columns, const std::vector<Number>& entries)
{
    if(rows*columns != entries.size())
    {
        std::cout<<"Failed to create Matrix Exception: Incomplete or excess entries";
        throw "exception";
    }
    boost::numeric::ublas::unbounded_array<Number> storage(entries.size());
    std::copy(entries.begin(), entries.end(),storage.begin());

    m_impl = boost::numeric::ublas::matrix<Number>(rows, columns, storage);
}

size_t
Matrix::rows( ) const
{
    return m_impl.size1();
}

size_t
Matrix::columns( ) const
{
    return m_impl.size2();
}

Number&
Matrix::operator()(size_t row, size_t column)
{
    return m_impl(row,column);
}

const Number&
Matrix::operator()(size_t row, size_t column) const
{
    return m_impl(row,column);
}

struct number_comparator
{
    bool operator()(const Number& lhs, const Number& rhs) const
    {
        return lhs == rhs;
    }
};

bool
Matrix::operator==(const Matrix& other) const
{
    return rows() == other.rows() && columns()==other.columns()
           && std::equal(m_impl.data().begin(),m_impl.data().end(),other.m_impl.data().begin(),number_comparator());
}

void
Matrix::print( ) const
{
    for (size_t row = 0; row < rows( ); ++row)
    {
        for (size_t column = 0; column < columns( ); ++column)
        {
            std::cout << m_impl(row, column)<<"\t";
        }
        std::cout << std::endl;
    }
    std::cout<<std::endl;
}

void
Matrix::input( )
{
    for (size_t row = 0; row < rows( ); ++row)
    {
        for (size_t column = 0; column < columns( ); ++column)
        {
            Number n;
            std::cin>>n;
            m_impl.insert_element(row,column,n);
        }
    }
}