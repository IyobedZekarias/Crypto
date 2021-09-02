#include "crypto.h"
#include <stdint.h>

using namespace crypto;
using namespace std;

int main()
{
    buffer_t buffer;
    
    //obviously you need to put stuff here
    urand(10, buffer);
    cout << "urand:  " << buffer; 
    buffer.clear(); 
    rdrand(11, buffer);
    cout << "rdrand: " << buffer; 
    buffer.clear(); 
    rdseed(12, buffer); 
    cout << "rdseed: " << buffer; 
    buffer.clear(); 
    randDev(15, buffer); 
    cout << "ranDev: " << buffer; 
    buffer.clear();
    LCG(12, buffer); 
    cout << "LCG:    " << buffer;  
    buffer.clear(); 

    urand(16, buffer); 
    rdrand(16, buffer); 
    rdseed(16, buffer); 
    randDev(16, buffer); 
    LCG(16, buffer); 
    cout << buffer; 


    
}

// int main(){ 
// }
