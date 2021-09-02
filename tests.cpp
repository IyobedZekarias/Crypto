#include <cassert>
#include <stdio.h>
#include "crypto.h"

using namespace crypto;
using namespace std;

int main()
{
    // obviously you need to put stuff here instead of the following
    buffer_t buffer; 
    int bytes = 4; 
    if(!urand(bytes, buffer)) std::cout << "urand failed" <<std::endl;
    else if(!rdrand(bytes, buffer)) std::cout << "rdrand failed" <<std::endl;
    else if(!rdseed(bytes, buffer)) std::cout << "rdseed failed" <<std::endl;
    else if(!randDev(bytes, buffer)) std::cout << "randdev failed" <<std::endl;
    else if(!LCG(bytes, buffer)) std::cout << "LCG failed" <<std::endl;
    else cout << "all tests passed" << std::endl;
}


// #include <cpuid.h>
// #include <iostream>
// #include <map>
// #include <string>

// using namespace std;



// int main() {
//     uint32_t eax, ebx, ecx, edx;
//     if (__get_cpuid(0x80000006, &eax, &ebx, &ecx, &edx)) {
//         printf("RDSEED %d\n", (ebx >> 18));
//         return 0;
//     } else {
//         printf("CPU does not support 0x80000006");
//         return 2;
//     }
// }