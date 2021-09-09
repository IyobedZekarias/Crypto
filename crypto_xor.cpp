#include "crypto.h"
#include <iomanip>

// NOTICE: there is NOT using namespace std here on purpose

namespace crypto {

   bool XOR(char* key, char* in, char* out){
        std::vector<u_char> buffer;
        std::vector<u_char> key_v;  
        std::ifstream inFile (in, std::ios::in | std::ios::binary); 
        if(inFile){
            inFile.seekg(0, inFile.end);
            long int filelength = inFile.tellg(); 
            inFile.seekg(0, inFile.beg); 
            buffer.resize(static_cast<size_t>(filelength));
            if(!inFile.read(reinterpret_cast<char*>(&buffer[0]), filelength)) return false; 

            inFile.close();
        } else return false;

        std::ofstream outFile(out, std::ios::out | std::ios::binary); 
        if(outFile){
            std::string key_str(key); 
            for( size_t i = 0; i < key_str.length(); i+=2)
            {
                u_char byte; 
                std::stringstream ss; 
                ss << key_str[i] << key_str[i+1]; 
                byte = static_cast<u_char>(std::stoul(ss.str(), nullptr, 16)); 
                key_v.push_back(byte); 
            }

            auto j = key_v.begin(); 
            for(auto i = buffer.begin(); i != buffer.end(); ++i){
                u_char a; 
                if(j == key_v.end()) a = *i ^ 0; 
                else {
                    a = *i ^ *j; 
                    j++;  
                }
                if(!outFile.write(reinterpret_cast<char*>(&a), 1)) return false; 
            }
            outFile.close(); 
            return true; 
         } else return false; 
    }

    
};
