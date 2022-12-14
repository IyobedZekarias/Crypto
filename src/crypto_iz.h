#ifndef _CRYPTO_H__
#define _CRYPTO_H__

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
#include <cassert>
#include <cstring>
#include "limits.h"
#include <iomanip>
#include <stdexcept>
#include <tgmath.h>
#include <exception>

namespace crypto {
    #define INT_ADD_OVERFLOW(a, b) \
        __builtin_add_overflow_p (a, b, static_cast<__typeof__ ((a) + (b))>(0))

     #define INT_SUB_OVERFLOW(a, b) \
        __builtin_sub_overflow_p (a, b, static_cast<__typeof__ ((a) + (b))>(0))
    
    typedef __int128 int128_t;
    typedef unsigned __int128 uint128_t;
    using buffer_t = std::vector<uint8_t>;
    class NNI {
    private: 
        std::vector<uint64_t> nni; 
        void push(uint64_t num); 
        void DeleteZeros(); 
        bool testbit(size_t ind) const;
        
    public:
        using digit_t = uint64_t;
        using b_digit_t = std::vector<digit_t>; 
        using l_digit_t = unsigned __int128;
        
        NNI(b_digit_t num); 
        NNI(crypto::buffer_t buf); 
        NNI(digit_t n);
        NNI(const char * str, int string=1); 
        NNI(); 
        crypto::buffer_t toBuffer() const; 
        
        //void divideby2();
        // NNI(const std::string &str);

        void randnni(size_t digits);
        size_t size() const;
        void clear(); 

        friend NNI divide(const NNI &u, const NNI &v, NNI &remainder);
        friend NNI modexp(const NNI &a, const NNI &e, const NNI &b);
        
        friend NNI operator+(const NNI &u,const NNI &v);
        friend NNI operator+=(const NNI &u,const NNI &v);
        friend void operator++(NNI &u);
        friend NNI operator-(const NNI &u,const NNI &v);
        friend void operator--(NNI &u);
        friend NNI operator-=(const NNI &u,const NNI &v);
        friend NNI operator*(const NNI &u,const NNI &v);
        friend NNI operator*=(const NNI &u,const NNI &v);
        friend NNI operator/(const NNI &u,const NNI &v);
        friend NNI operator/=(const NNI &u,const NNI &v);
        friend NNI operator%(const NNI &u,const NNI &v);
        friend NNI operator%=(const NNI &u,const NNI &v);
        NNI operator=(const digit_t &v);
        NNI operator=(const b_digit_t &v);

        friend bool operator<(const NNI &u,const NNI &v);
        friend bool operator<=(const NNI &u,const NNI &v);
        friend bool operator>(const NNI &u,const NNI &v);
        friend bool operator>=(const NNI &u,const NNI &v);
        friend bool operator==(const NNI &u,const NNI &v);
        friend bool operator==(const NNI &u,const NNI::digit_t dig);
        friend bool operator!=(const NNI &u,const NNI &v);

        friend NNI operator>>(const NNI &u, NNI::digit_t v);
        friend NNI operator<<(const NNI &u, NNI::digit_t v);
        friend NNI operator&(const NNI &u, const NNI &v);

        digit_t& operator[](const int i);
        friend std::ostream & operator<<(std::ostream &out,const NNI &priv);

    };
    

    typedef struct RSApublic64 {
        uint32_t e = 0; 
        uint128_t n; 
    } RSApublic64;

    typedef struct RSAprivate64 { 
        uint128_t d;
        uint64_t p = 0; 
        uint64_t q = 0;
        uint128_t n;
        uint128_t phi;
    } RSAprivate64; 

    typedef struct RSApublic {
        NNI * e = (NNI*)malloc(sizeof(e)); 
        NNI * n = (NNI*)malloc(sizeof(n)); 
    } RSApublic;

    typedef struct RSAprivate { 
        NNI * d = (NNI*)malloc(sizeof(d));
        NNI * p = (NNI*)malloc(sizeof(p)); 
        NNI * q = (NNI*)malloc(sizeof(q));
        NNI * n = (NNI*)malloc(sizeof(n));
        NNI * phi = (NNI*)malloc(sizeof(phi));
    } RSAprivate; 

    // Alice:   s = generate()
    // Alice:   e = encode(s,A's private key)
    //      Alice --e--> Bob
    // Bob:     t = generate(e)
    // Bob:     f = encode(t,B's private key)
    //      Alice <--f-- Bob
    // Bob:     k = combine(e,t,A's public key)
    // Alice:   k = combine(f,s,B's public key)
    typedef struct dh_secret_t {
        NNI p;      // prime
        NNI g;      // base, usually 2
        NNI a;      // my half of the secret, any random number
    } dh_secret_t;

    typedef struct dh_exchange_t {
        NNI p;        // prime
        NNI g;        // base, usually 2
        NNI half;  // encoded half secret; g^a mod p
        NNI sig;    // authentication; hash(half)^priv.d mod priv.n
    } dh_exchange_t;

    #define cpuid(func,ax,bx,cx,dx)\
     __asm__ __volatile__ ("cpuid":\
     "=a" (ax), "=b" (bx), "=c" (cx), "=d" (dx) : "a" (func)); 

    using block_t = __m128i;
    /*
        bit 18 in the ebx register has to be turned on intel chips
    */
    int Check_CPU_support_RDSEED();
    /*
        bit 30 in the ecx register has to be turned on intel chips
    */
    int Check_CPU_support_RDRAND();
    /*
        bit 18 in the ebx register has to be turned on intel chips
    */
    int Check_CPU_support_AES(); 
   
    /*
        urand will take a number of bytes as the argument and will return a buffer
        New data will be appended on to the end of the buffer and anything that is currently in the buffer 
            will not be deleted 
        The amount of new data will be equal to the number of bytes
        Urand reads from the OS urandom file
    */
    bool urand(size_t bytes,buffer_t &buffer);

    /*
        rdrand and rdseed both perform the same way
        both functions use intel cpu commands to read random data
        both functions use entropy of the cpu to find random numbers
        cpu must be able to call rdrand and rdseed for these to work
        both functions do not erase what is in the buffer neither and append the 
            exact number of bytes into buffer
    */
    bool rdrand(size_t bytes, buffer_t &buffer); 
    bool rdseed(size_t bytes, buffer_t &buffer); 
    /*
        Generates random numbers using c random device
        bytes is the desired number of bytes to be input into buffer
        New data will be appended on to the end of the buffer and anything that is currently in the buffer 
            will not be deleted 
        The amount of new data will be equal to the number of bytes
    */
    bool randDev(size_t bytes, buffer_t &buffer); 

    /*
        LCG is a linear congruence generator that mathamatically generates bytes that
            seem to be random but not actually
        LCG will take the desired number of bytes and append the exact amount to the buffer
        If you would like to specify a seed you can and add an 8 byte seed, if you do this everytime
            LCG is called it will run the same way with the same numbers
            Otherwise a random seed will be generated using rdrand
    */
    bool LCG(size_t bytes, buffer_t &buffer); 
    bool LCG(size_t bytes, buffer_t &buffer, uint32_t seed);
    /*
        XOR will take a key, input file, and output file
        the input file should be a binary file
        the output file does not need to exist but if it does it should be a binary file as well 
    */
    bool XOR(const buffer_t key, char* in, char* out);
   /*
        XOR will take a buffer and output to a file
        The buffer is the same datatype as the rest of the library
        Contents of a buffer should be hexadecimal elements
        This function will xor a key and an input and write it to a file
    */
    bool XOR(const buffer_t key, buffer_t in, char* out); 
   /*
        XOR will take a buffer and write to a buffer
        The buffer is the same datatype as the rest of the library
        Contents of the input buffer should be hexadecimal elements
        This function is best if you want to do other computation with the output
    */
    bool XOR(const buffer_t key, buffer_t in, buffer_t &out); 
     /*
        XOR will take a file and write to a buffer
        The buffer is the same datatype as the rest of the library
        The chosen file should be a binary file
        This function is great if you want to do other computation with the output
    */
    bool XOR(const buffer_t key, char* in, buffer_t &out);
    /*
        Readfile takes the name of the file and returns the contents of the file in a buffer
    */
    bool readFile(const char* fileName, buffer_t &buffer); 
    /*
        writefile takes the file name and a buffer and writes the contents of that buffer 
        and puts them into the file specified
    */
    bool writeFile(const char* fileName, buffer_t buffer);
    /*
        Read a .iyo file and put it into a public key
    */
    bool readFile(const char* filename, RSAprivate &priv);
   /*
        Write private key to a .iyo file
   */
    bool writeFile(const char * filename, RSAprivate &priv);
    /*
        Write public key to a .iyo file
    */
   bool writeFile(const char * filename, RSAprivate &priv);
    /*
        Read a .iyo file and put it into public key
    */
    bool readFile(const char* filename, RSApublic &pub);
    /*
        Write dh_secret to file
    */
    bool writeFile(const char* filename, dh_secret_t &priv);
    /*
       Write dh_exchange to file
    */
    bool writeFile(const char* filename, dh_exchange_t &pub);
    /*
        Read a .zek file and put it into dh_secret
    */
    bool readFile(const char* filename, dh_secret_t &priv);
    /*
        Read a .zek file and put it into dh_exchange
    */
    bool readFile(const char* filename , dh_exchange_t &pub);
    /*
        ToBuffer takes a string and converts it into a buffer
        Best done with a key 
        For example 00F12A would become a buffer with elements 00 F1 2A
    */
    void ToBuffer(char* array, buffer_t &buffer); 
    /*
        Makes a file filled with random data
    */
    void MakeBinFile(char * fileName, size_t bytes); 
    /*
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
    bool encode_aes128_ecb(const buffer_t &plain, buffer_t key, buffer_t &cipher); 
    /*
        decode will use aes to decrypt the cipher buffer with the key buffer and put decoded
            data into the plain buffer
        decode takes all buffers as arguments, 
            if your data is in a file call readFile and put data into a buffer
            before calling decode
        decode will not change the cipher buffer but will clear out anything in the plain 
            buffer before filling it
        decode will return false if cpu does not support intel aes intrinsics
    */
    bool decode_aes128_ecb(const buffer_t &cipher,buffer_t key, buffer_t &plain);
    /*
        encode aes cbc is like aes encode ecb but it takes an initial value (IV) as an argument and 
            XORs the initial value with the first block of the plaintext and XORs the first block of the ciphertext
            with the next block of the plain text and so on
        If you do not specify the value of the IV then the IV will be randomly computed, to do this pass an empty vector into 
            IV and it will be populated with random values 
        You should then save the values of IV somewhere as you will need it for decoding
        NOTE: If IV has values in it and is not equal to the size of a block (16 bytes) then the function will return false

    */
    bool encode_aes128_cbc(const buffer_t &plain, buffer_t key, buffer_t &cipher, buffer_t &IV);

    /*
        decode aes cbc is like decode aes ecb but it takes a n initial value (IV)
        This IV has to be the same IV that was used in the encoding process, otherwise the computed plaintext will not 
            have the expected value 
        If you did not save the IV after encoding tough luck
        NOTE: If IV does not have a size of one block (16 bytes) the function will return false
    */
    bool decode_aes128_cbc(const buffer_t &plain, buffer_t key, buffer_t &cipher, buffer_t &IV);
    /*
        This function utilizes both SHA2 512 SHA2 512/t
        A buffer with the plain text will be taken as the first argument
        The second argument should be an empty buffer that will be filled 
        The final argument for t is the amount of bits that should be truncated off of the hashed value 
            the rule fo t is that it should be between 100 and 512 inclusive
        If you want to do just use SHA512 without truncating any values leave t as 0
    */
    bool hash_sha512(const buffer_t &plain,buffer_t &hash, int t = 0);
    
    //TODO: make this two functions
    /*
    
        Uses Rabin-Miller primality test implementation from tutorialspoint
            https://www.tutorialspoint.com/cplusplus-program-to-implement-the-rabin-miller-primality-test-to-check-if-a-given-number-is-prime
        This function will generate two prime number nad multiply them together to make an 
            n value. This n value will be put into the public key and the p and q primes will be saved 
            in the private key 
        Additionally (p-1) * (q-1) = phi. Phi will be saved in the private key
        The function will use 65537 as the value for e that will be in the public key
        Using the Extended Euclidean Algorithm the d value will also be generated such that 
            ed = 1 mod phi
        
        In summary the public key will contain 
            e = 65537 and n 
        And the private key will contain 
            prime p, prime q, phi = (p-1) * (q-1), and d = e^-1 modp phi, and n (for convenience)
        
    */
    bool generate_rsa64(RSAprivate64 &key, RSApublic64 &pub);
    /*
        encode rsa will use a simple algorithm to encode a message
        the only constraint is that the numeric representation of the message has to be 
            smaller than the value of pub.n
        The function will put the ciphered message back into cipher
        The RSApublic key will have to have been generated with generate_rsa
    */
    bool encode_rsa64(const buffer_t message, buffer_t &cipher, const RSApublic64 &pub);
    /*
        decoe will take a cipher that was made by enocde then will decode it
        The RSAprivate key will have to have been generated with generate_rsa
        @ params Hi 
    */ 
    bool decode_rsa64(buffer_t &message, const buffer_t cipher, const RSAprivate64 &key);
    bool Miller(NNI p, int iteration);
    bool generate_rsa(RSAprivate *key, RSApublic *pub); 
    bool encode_rsa(const buffer_t message, buffer_t &cipher, const RSApublic &pub); 
    bool decode_rsa(buffer_t &message, const buffer_t cipher, const RSAprivate &key);


    // create a new half of a DH secret, if e is given check it and be compatible with it (else fail)
    bool dh_generate(dh_secret_t &secret, const dh_exchange_t *e = nullptr);

    // using half a DH secret and our own private key, make an encoded half
    bool dh_encode(dh_exchange_t &e, const dh_secret_t &secret, const RSAprivate &key);

    // verify an encode half and combine with our secret to create the DH shared secret
    bool dh_combine(NNI &shared_secret,const dh_exchange_t &e,const dh_secret_t &secret,const RSApublic &pub);

};

std::ostream & operator<<(std::ostream &out,const crypto::buffer_t &buffer);
std::ostream & operator<<(std::ostream &out,const crypto::RSApublic64 &pub);
std::ostream & operator<<(std::ostream &out,const crypto::RSAprivate64 &priv);
std::ostream & operator<<(std::ostream &out,const crypto::RSApublic &pub);
std::ostream & operator<<(std::ostream &out,const crypto::RSAprivate &priv);


#endif
