#include "../src/crypto_iz.h"
#include <stdint.h>

using namespace crypto;
using namespace std;

int main(int argc, char** argv)
{
    if(argc < 7){
        cout << "Comand line arguments not found" << std::endl << "   Enter <# of bytes> <urand? 1:0> <rdrand? 1:0> <rdseed? 1:0> <random device? 1:0> <LCG? 1:0>" << std::endl;
        return 0; 
    }
    buffer_t buffer;
    
    // //obviously you need to put stuff here
    // urand(10, buffer);
    // cout << "urand:  " << buffer; 
    // buffer.clear(); 
    // rdrand(11, buffer);
    // cout << "rdrand: " << buffer; 
    // buffer.clear(); 
    // rdseed(12, buffer); 
    // cout << "rdseed: " << buffer; 
    // buffer.clear(); 
    // randDev(15, buffer); 
    // cout << "ranDev: " << buffer; 
    // buffer.clear();
    // LCG(12, buffer); 
    // cout << "LCG:    " << buffer;  
    // buffer.clear(); 
    int totBytes = 0, bytes = stoi(argv[1]); 

    if(stoi(argv[2])) if(urand(bytes, buffer)) totBytes += bytes; 
    if(stoi(argv[3])) if(rdrand(bytes, buffer)) totBytes += bytes; 
    if(stoi(argv[4])) if(rdseed(bytes, buffer)) totBytes += bytes; 
    if(stoi(argv[5])) if(randDev(bytes, buffer)) totBytes += bytes;  
    if(stoi(argv[6])) if(LCG(bytes, buffer)) totBytes += bytes;  
    cout << buffer; 


    
}
