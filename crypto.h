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

    bool XOR(char* key, char* in, char* out); /*
        XOR will take a key, input file, and output file
        the key needs to be in hexadimal format (e.g. 00FAC1)
        the input file should be a binary file
        the output file does not need to exist but if it does it should be a binary file as well 
    */
    
};

std::ostream & operator<<(std::ostream &out,const crypto::buffer_t &buffer);


#endif
