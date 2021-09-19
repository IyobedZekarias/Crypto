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
#include <wmmintrin.h>
#include <cpuid.h> 
#include <bitset>

#ifndef _CRYPTO_H
#define _CRYPTO_H

namespace crypto {
    using buffer_t = std::vector<uint8_t>;

    #define cpuid(func,ax,bx,cx,dx)\
     __asm__ __volatile__ ("cpuid":\
     "=a" (ax), "=b" (bx), "=c" (cx), "=d" (dx) : "a" (func)); 

    using block_t = __m128i;

    int Check_CPU_support_RDSEED(); /*
        bit 18 in the ebx register has to be turned on intel chips
    */
    int Check_CPU_support_AES(); /*
        bit 18 in the ebx register has to be turned on intel chips
    */

    bool urand(size_t bytes,buffer_t &buffer);/*
        urand will take a number of bytes as the argument and will return a buffer
        New data will be appended on to the end of the buffer and anything that is currently in the buffer 
            will not be deleted 
        The amount of new data will be equal to the number of bytes
        Urand reads from the OS urandom file
    */

    bool rdrand(size_t bytes, buffer_t &buffer); 
    bool rdseed(size_t bytes, buffer_t &buffer); /*
        rdrand and rdseed both perform the same way
        both functions use intel cpu commands to read random data
        both functions use entropy of the cpu to find random numbers
        cpu must be able to call rdrand and rdseed for these to work
        both functions do not erase what is in the buffer neither and append the 
            exact number of bytes into buffer
    */

    bool randDev(size_t bytes, buffer_t &buffer); /*
        Generates random numbers using c random device
        bytes is the desired number of bytes to be input into buffer
        New data will be appended on to the end of the buffer and anything that is currently in the buffer 
            will not be deleted 
        The amount of new data will be equal to the number of bytes
    */

    bool LCG(size_t bytes, buffer_t &buffer); 
    bool LCG(size_t bytes, buffer_t &buffer, uint32_t seed);/*
        LCG is a linear congruence generator that mathamatically generates bytes that
            seem to be random but not actually
        LCG will take the desired number of bytes and append the exact amount to the buffer
        If you would like to specify a seed you can and add an 8 byte seed, if you do this everytime
            LCG is called it will run the same way with the same numbers
            Otherwise a random seed will be generated using rdrand
    */

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

    void MakeBinFile(char * fileName, size_t bytes); /*
        Makes a file filled with random data
    */

    bool encode_aes128_ecb(const buffer_t &plain, buffer_t key, buffer_t &cipher); /*
        enocdoe will use aes to encrypt plain buffer with the key buffer and put encrypted
            data into the cipher buffer
        encode takes all buffers as arguments, 
            if your data is in a file call readFile and put data into a buffer
            before calling encode
        encode will not change the plain buffer but will clear out anything in the cipher 
            buffer before filling it
        encode will return false if cpu does not support intel aes intrinsics
        if plain buffer length in bytes is not multiple of 16
            encode will pad the end of the plaintext without changing the plain buffer
            the padding will make the length of the plaintext equal to a multiple of 16
    */

    bool decode_aes128_ecb(const buffer_t &cipher,buffer_t key, buffer_t &plain);/*
        decode will use aes to decrypt the cipher buffer with the key buffer and put decoded
            data into the plain buffer
        decode takes all buffers as arguments, 
            if your data is in a file call readFile and put data into a buffer
            before calling decode
        decode will not change the cipher buffer but will clear out anything in the plain 
            buffer before filling it
        decode will return false if cpu does not support intel aes intrinsics
    */
    
};

std::ostream & operator<<(std::ostream &out,const crypto::buffer_t &buffer);


#endif
