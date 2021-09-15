#include "crypto.h"
#include <iomanip>


namespace crypto {
    
    bool XOR(const buffer_t key, buffer_t in, buffer_t &out){
        auto j = key.begin(); 
        for(auto i = in.begin(); i != in.end(); ++i){
            u_char a; 
            if(j == key.end()) a = *i ^ 0; 
            else {
                a = *i ^ *j; 
                j++;  
            }
            out.push_back(a); 
        }
        return true; 
    }

    bool XOR(const buffer_t key, char* in, buffer_t &out){
        buffer_t in_v; 

        if(!readFile(in, in_v)) return false; 

        return XOR(key, in_v, out); 
    }

    bool XOR(const buffer_t key, buffer_t in, char* out){
        buffer_t cipherText; 

        XOR(key, in, cipherText);

        if(!writeFile(out, cipherText)) return false; 
        return true;
    }

    bool XOR(const buffer_t key, char* in, char* out){
        buffer_t in_v; 
        buffer_t cipherText;  

        if(!readFile(in, in_v)) return false; 

        XOR(key, in_v, cipherText); 

        if(!writeFile(out, cipherText)) return false; 
        
        return true;  
    }

    
};
