#include "../src/crypto_iz.h"
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

        buffer_t IV; 
        encode_aes128_cbc(plaintext, key, cipher, IV); 

        //cipher.resize(cipher.size() + IV.size()); 
        cipher.insert(cipher.end(), IV.begin(), IV.end()); 
        writeFile(argv[3], cipher); 
        cipher.resize(cipher.size() - 16); 
        plaintext.clear(); 
        plaintext.resize(0); 

        decode_aes128_cbc(cipher, key, plaintext, IV); 

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

        buffer_t IV; 
        encode_aes128_cbc(plaintext, key, cipher, IV);

        cipher.insert(cipher.end(), IV.begin(), IV.end()); 
        writeFile(argv[3], cipher); 
    }
    else if(std::string(argv[4]) == "decode"){
        buffer_t key, plaintext, cipher; 
        readFile(argv[1], key);
        readFile(argv[2], cipher);
        buffer_t IV; 

        int IVsize = 16; 
        for(auto i = cipher.rbegin(); i != cipher.rend(), IVsize > 0; ++i, IVsize--){
            IV.push_back(*i); 
            cipher.erase((i+1).base()); 
        } 
        std::reverse(IV.begin(), IV.end());  

        decode_aes128_cbc(cipher, key, plaintext, IV); 

        writeFile(argv[3], plaintext); 
    }
    
}
