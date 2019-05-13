
#include "Matrix.h"
#include "MatrixAlgorithms.h"

int
main( int argc, char const* argv[] )
{
    std::cout << "Enter the sizes of rows and columns " << std::endl;

    uint32_t rows;
    uint32_t columns;

    std::cin >> rows >> columns;

    Matrix mat( rows, columns );
    std::cout << "Matrix of " << mat.rows( ) << "x" << mat.columns( ) << std::endl;

    mat.input( );

    std::cout << std::endl;
    mat.print( );

    if ( rref( mat ) )
    {
        std::cout << "The Reduced Row Rchelon Form (RREF) of your matrix is:" << std::endl;
        mat.print( );
    }
    else
    {
        std::cout << " Error: Matrix could not be reduced" << std::endl;
    }

    return 0;
}