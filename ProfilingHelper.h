// Trick back-end collection for readable, yet profilable executions

#pragma once

#include <chrono>
#include <functional>

using Duration = std::chrono::duration< double >;

class Matrix;

/* Trick to make execution of all computation with profiling withouting putting explicit markers
 * around the calling function. The purpose of the function is simple. Call the given function with
 * the arguments and add time to the timekeeper variable. The down-side of the perfect forwarding
 * variadic arguments is that the cast has to be specified in the place this function is called.
 * However, that's a trade-off for readability without skipping the profiling.
 */
template < typename... Arguments >
void
execute_with_profiling( void ( *computation )( Matrix&, Arguments... ),
                        Duration& timekeeper,
                        Matrix& mat,
                        Arguments&&... args )
{
    auto tick = std::chrono::high_resolution_clock::now( );
    computation( mat, std::forward< Arguments >( args )... );
    timekeeper += ( std::chrono::high_resolution_clock::now( ) - tick );
}

// This is basically the same as above, only with a return type
template < typename Return, typename... Arguments >
Return
execute_with_profiling( Return ( *computation )( Matrix&, Arguments... ),
                        Duration& timekeeper,
                        Matrix& mat,
                        Arguments&&... args )
{
    auto tick = std::chrono::high_resolution_clock::now( );
    Return result = computation( mat, std::forward< Arguments >( args )... );
    timekeeper += ( std::chrono::high_resolution_clock::now( ) - tick );
    return result;
}

void
publish_metrics( Duration elapsed,
                 Duration pivoting,
                 Duration max_abs_value,
                 Duration swapping,
                 Duration normalization,
                 Duration gauss_elimination )
{
    std::cout << "Total execution Time : " << elapsed.count( ) << std::endl;
    std::cout << "Pivoting: " << ( pivoting.count( ) / elapsed.count( ) ) * 100 << "/% "
              << "Split: "
              << "Getting max entry: " << ( max_abs_value.count( ) / pivoting.count( ) ) * 100
              << "/% "
              << "Swapping Rows: " << ( swapping.count( ) / pivoting.count( ) ) * 100 << "/% "
              << std::endl;
    std::cout << "Normalization: " << ( normalization.count( ) / elapsed.count( ) ) * 100 << "/%"
              << std::endl;
    std::cout << "Gauss Elimination: " << ( gauss_elimination.count( ) / elapsed.count( ) ) * 100
              << "/%" << std::endl;
}