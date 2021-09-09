# C Container Library
C++ provides classes for dynamic data allocation, such as list, vector. However, there is no container for C. So, this library is to provide the container list, array, etc. for any data type with same size and it is implement in C. The data location is allocated through malloc, so it can store different data type. However, the size of each element in the container has the same data size.

## How to use
Copy the *.h and the correspond *.c file to the ./include folder.

## TODO List
1. Separate cctrlib and test folder, modify makefile
2. Add function explanation
3. Use unittest library to test all available API.
4. Add different container type (single linked list, fifo, array, etc.)
