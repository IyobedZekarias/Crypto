#include "crypto.h"
#include <cstring>

using namespace crypto;
using namespace std; 

int main(int argc, char** argv){
    if(argc != 5){
        std::cout << "Usage: " << std::string(argv[0]) << " <key file> <input file> <output file> <\"encode\" | \"decode\">" << std::endl; 
        return 0; 
    }

    if(std::string(argv[4]) == "demo"){
        buffer_t key, plaintext, cipher; 
        readFile(argv[1], key);
        readFile(argv[2], plaintext);  

        encode_aes128_ecb(plaintext, key, cipher); 

        writeFile(argv[3], cipher); 
        plaintext.clear(); 
        plaintext.resize(0); 

        decode_aes128_ecb(cipher, key, plaintext); 

        std::string plainFile(argv[2]); 
        plainFile.append("2");
        char second_File[plainFile.length()]; 
        strcpy(second_File, plainFile.c_str()); 
        writeFile(second_File, plaintext); 
    }
    else if(std::string(argv[4]) == "encode"){
        buffer_t key, plaintext, cipher; 
        readFile(argv[1], key);
        readFile(argv[2], plaintext);  

        encode_aes128_ecb(plaintext, key, cipher); 

        writeFile(argv[3], cipher); 
    }
    else if(std::string(argv[4]) == "decode"){
        buffer_t key, plaintext, cipher; 
        readFile(argv[1], key);
        readFile(argv[2], cipher);
        decode_aes128_ecb(cipher, key, plaintext); 

        writeFile(argv[3], cipher); 
    }
    
}
