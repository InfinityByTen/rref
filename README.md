# rref
A c++ app to calculate the RREF of a mxn matrix of rational numbers.

Currently build on a mac environment using boost and GMP Rational Numbers back-end.

This code is published with a release directly compiled on my local machine.
I need to still add a build system to the code.

To compile, one can try with the following.

   g++ -std=c++11 -I ($GPP_PATH) -L ($LIBRARY_PATH) main.cpp Matrix.cpp -lgmp -o App
   
To compile the suite of tests for the algorithm which are also have profile information

  g++ -std=c++11 -I ($GPP_PATH) -L ($LIBRARY_PATH) AlgorithmTest.cpp Matrix.cpp -lgmp -o ProfiledTests
  
