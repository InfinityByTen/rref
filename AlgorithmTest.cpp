// Test Suite for checking the Row Reduced Echelon Form Engine

#include "MatrixAlgorithms.h"

#include <string>

void
test_example( std::string test_name,
              size_t row,
              size_t column,
              const std::vector< Number >& input,
              const std::vector< Number >& expected )
{
    Matrix mat1( row, column, input );
    Matrix mat2( row, column, expected );
    std::cout << test_name << ":" << std::endl;
    rref( mat1 );
    std::cout << ( mat1 == mat2 ? "Test Passed" : "Test Failed" ) << std::endl << std::endl;
}

int
main( int argc, char const* argv[] )
{
    // check basic gauss elimination
    std::vector< Number > simple{1, 2, 3, 4};
    std::vector< Number > simple_rref{1, 0, 0, 1};
    test_example( "Basic Gauss Elimination", 2, 2, simple, simple_rref );

    // rank deficient matrix
    std::vector< Number > rank_deficient{16, 2, 3, 13, 5, 11, 10, 8, 9, 7, 6, 12, 4, 14, 15, 1};
    std::vector< Number > rank_deficient_rref{1, 0, 0, 1, 0, 1, 0, 3, 0, 0, 1, -3, 0, 0, 0, 0};
    test_example( "Rank Deficient RREF", 4, 4, rank_deficient, rank_deficient_rref );

    // check pivoting works
    std::vector< Number > division_by_zero{1, -1, 2, -1, 2, -2, 3, -3, 1, 1, 1, 0, 1, -1, 4, 3};
    std::vector< Number > division_by_zero_rref{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    test_example( "Pivoting to Avoid didving by Zero", 4, 4, division_by_zero,
                  division_by_zero_rref );

    // check rref is working
    std::vector< Number > basic3{8, 1, 6, 3, 5, 7, 4, 9, 2};
    std::vector< Number > basic3_rref{1, 0, 0, 0, 1, 0, 0, 0, 1};
    test_example( "Complete RREF", 3, 3, basic3, basic3_rref );

    // check for rectangular matrices
    std::vector< Number > rectangular{0, 3, -6, 6, 4, -5, 3, -7, 8, -5, 8, 9, 3, -9, 12, -9, 6, 15};
    std::vector< Number > rectangular_rref{1, 0, -2, 3, 0, -24, 0, 1, -2,
                                           2, 0, -7, 0, 0, 0,   0, 1, 4};
    test_example( "Rectangular Matrix", 3, 6, rectangular, rectangular_rref );

    return 0;
}