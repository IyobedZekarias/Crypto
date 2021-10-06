#include "crypto.h"
#include <cstring>

using namespace crypto;
using namespace std; 

int main(int argc, char** argv){
    if(argc < 4){
        std::cout << "Usage: " << std::string(argv[0]) << "<input file> <output file> <\"512\" | \"512/t\">\n    where `t` is the number of bits you would like returned -- must be between 100 and 512 inclusive" << std::endl; 
        return 0; 
    }

    if(std::string(argv[3]) == "demo"){
        buffer_t plaintext, hash; 
        if(std::string(argv[4]) == "t"){
            readFile(argv[1], plaintext);  

            hash_sha512(plaintext, hash, 256);
            
            writeFile(argv[2], hash); 
        }else{
            readFile(argv[1], plaintext);  

            hash_sha512(plaintext, hash, 0);
            
            writeFile(argv[2], hash); 
        }
    }
    else if(std::string(argv[3]) == "512"){
        buffer_t plaintext, hash; 
        readFile(argv[1], plaintext);  

        hash_sha512(plaintext, hash, 0);
        
        writeFile(argv[2], hash); 
        
    }
    else if(argv[3][3] == '/'){
        int t = (static_cast<int>(argv[3][4])*100) + (static_cast<int>(argv[3][5])*10) + static_cast<int>(argv[3][6]); 
        buffer_t plaintext, hash; 
        readFile(argv[1], plaintext);  

        hash_sha512(plaintext, hash, t);
        
        writeFile(argv[2], hash); 
        
    }
    
}
