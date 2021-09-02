#include <cassert>
#include "crypto.h"

using namespace crypto;
using namespace std;

int main()
{
    // obviously you need to put stuff here instead of the following
    buffer_t buffer; 
    int bytes = 4; 
    assert(urand(bytes, buffer));
    assert(rdrand(bytes, buffer));
    //assert(rdseed(bytes, buffer));
    assert(randDev(bytes, buffer));
    assert(LCG(bytes, buffer));
    cout << "all tests passed" << std::endl;
}
