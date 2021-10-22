#include "crypto.h"
#include <stdint.h>

using namespace crypto;
using namespace std;

int main(int argc, char** argv)
{
    // if(argc < 7){
    //     cout << "Usage: " << std::string(argv[0]) << "" << endl;
    //     return 0; 
    // }
   
    RSAprivate64 priv; 
    RSApublic64 pub; 
    // pub.e = 3; 
    // priv.p = 173; 
    // priv.q = 149;
    generate_rsa64(priv, pub); 

    // std::cout << pub; 
    // std::cout << priv; 

    buffer_t message; 
    buffer_t cipher; 
    readFile(argv[1], message);
    //std::cout << message << std::endl; 
    assert(encode_rsa64(message, cipher, pub)); 
    assert(decode_rsa64(message, cipher, priv));
    //std::cout << message; 


    
}
