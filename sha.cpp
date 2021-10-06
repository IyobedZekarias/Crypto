#include "crypto.h"
#include <cstring>

using namespace crypto;
using namespace std; 

int main(int argc, char** argv){
    if(argc != 4){
        std::cout << "Usage: " << std::string(argv[0]) << "<input file> <output file> <\"512\" | \"512/t\">\n    where `t` is the number of bits you would like returned -- must be between 100 and 512 inclusive" << std::endl; 
        return 0; 
    }

    if(std::string(argv[3]) == "demo"){
        buffer_t plaintext = {0x61, 0x62, 0x63}; 
        buffer_t Writtenhash = {0xdd, 0xaf, 0x35, 0xa1, 0x93, 0x61, 0x7a, 0xba, 0xcc, 0x41, 0x73, 0x49, 0xae, 0x20, 0x41, 0x31, 0x12, 0xe6,
                                0xfa, 0x4e, 0x89, 0xa9, 0x7e, 0xa2, 0x0a, 0x9e, 0xee, 0xe6, 0x4b, 0x55, 0xd3, 0x9a, 0x21, 0x92, 0x99, 0x2a,
                                0x27, 0x4f, 0xc1, 0xa8, 0x36, 0xba, 0x3c, 0x23, 0xa3, 0xfe, 0xeb, 0xbd, 0x45, 0x4d, 0x44, 0x23, 0x64, 0x3c, 
                                0xe8, 0x0e, 0x2a, 0x9a, 0xc9, 0x4f, 0xa5, 0x4c, 0xa4, 0x9f};
        buffer_t computedHash; 
        hash_sha512(plaintext, computedHash, 0);
        cout << ".";
        cout << computedHash; 
        // buffer_t plaintext, hash; 
        // readFile(argv[1], plaintext);  

        // hash_sha512(plaintext, hash, 0);
        
        // writeFile(argv[2], hash); 
    }
    else if(std::string(argv[3]) == "512"){
        buffer_t plaintext, hash; 
        readFile(argv[1], plaintext);  

        hash_sha512(plaintext, hash, 0);
        
        writeFile(argv[2], hash); 
        
    }
    else if(argv[3][3] == '/'){
        int t = (static_cast<int>(argv[3][4])*100) + (static_cast<int>(argv[3][5])*10) + static_cast<int>(argv[3][6]); 
        std::cout << t; 
        
    }
    
}
