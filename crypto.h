#include <iostream>
#include <random> 
#include <math.h>
#include <vector>
#include <cstddef>
#include <unistd.h>
#include <fstream> 
#include <iterator>
#include <algorithm>
#include <immintrin.h>
#include <cpuid.h> 
#include <bitset>

#ifndef _CRYPTO_H
#define _CRYPTO_H

namespace crypto {
    using buffer_t = std::vector<uint8_t>;

    bool urand(size_t bytes,buffer_t &buffer);
    bool rdrand(size_t bytes, buffer_t &buffer); 
    bool rdseed(size_t bytes, buffer_t &buffer); 
    bool randDev(size_t bytes, buffer_t &buffer); 
    bool LCG(size_t bytes, buffer_t &buffer); 
    bool LCG(size_t bytes, buffer_t &buffer, uint32_t seed);

    bool XOR(const buffer_t key, char* in, char* out); /*
        XOR will take a key, input file, and output file
        the input file should be a binary file
        the output file does not need to exist but if it does it should be a binary file as well 
    */
    bool XOR(const buffer_t key, buffer_t in, char* out); /*
        XOR will take a buffer and output to a file
        The buffer is the same datatype as the rest of the library
        Contents of a buffer should be hexadecimal elements
        This function will xor a key and an input and write it to a file
    */
    bool XOR(const buffer_t key, buffer_t in, buffer_t &out); /*
        XOR will take a buffer and write to a buffer
        The buffer is the same datatype as the rest of the library
        Contents of the input buffer should be hexadecimal elements
        This function is best if you want to do other computation with the output
    */

    bool XOR(const buffer_t key, char* in, buffer_t &out); /*
        XOR will take a file and write to a buffer
        The buffer is the same datatype as the rest of the library
        The chosen file should be a binary file
        This function is great if you want to do other computation with the output
    */

    bool readFile(char* fileName, buffer_t &buffer); /*
        Readfile takes the name of the file and returns the contents of the file in a buffer
    */
    bool writeFile(char* fileName, buffer_t buffer); /*
        writefile takes the file name and a buffer and writes the contents of that buffer 
        and puts them into the file specified
    */
    void ToBuffer(char* array, buffer_t &buffer); /*
        ToBuffer takes a string and converts it into a buffer
        Best done with a key 
        For example 00F12A would become a buffer with elements 00 F1 2A
    */

    
};

std::ostream & operator<<(std::ostream &out,const crypto::buffer_t &buffer);


#endif
