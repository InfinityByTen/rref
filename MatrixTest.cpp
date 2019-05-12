// Test for Matrix Class

#include "Matrix.h"

void
test_constructor(std::string test_name, bool expectation, size_t rows, size_t columns, std::vector<Number> entries)
{
    std::cout<<test_name<<": ";
    try
    {
        Matrix(rows, columns, entries );
    }
    catch(...)
    {
        std::cout<< (!expectation ? " Passed" : " Failed")<<std::endl;
        return;
    }
    std::cout<<"Passed"<<std::endl;
}

int main(int argc, char const *argv[])
{
    std::vector<Number> entries{1, 2, 3, 4};
    test_constructor("Correct Constructor", true, 2, 2, entries);

    std::vector<Number> entries_wrong{1, 2, 3};
    test_constructor("Incorrect Throws exception", false, 2, 2, entries_wrong);

    return 0;
}