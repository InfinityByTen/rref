
#include "Matrix.h"

int main(int argc, char const *argv[])
{
    std::cout<<"Enter the sizes of rows and columns "<<std::endl;

    uint32_t rows;
    uint32_t columns;

    std::cin>>rows>>columns;

    Matrix mat(rows, columns);
    std::cout<<"Matrix of "<<mat.size1()<<"x"<<mat.size2()<<std::endl;

    mat.input();
    mat.print();

    return 0;
}