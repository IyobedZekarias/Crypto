#include "../src/crypto.h"
#include <stdint.h>

using namespace crypto;
using namespace std;

int main(int argc, char** argv){
    RSAprivate AlicePriv; 
    RSApublic AlicePub; 
    RSAprivate BobPriv; 
    RSApublic BobPub; 

    readFile("Apriv.iyo", AlicePriv); 
    readFile("Apub.iyo", AlicePub); 
    readFile("Bpriv.iyo", BobPriv); 
    readFile("Bpub.iyo", BobPub); 

    dh_secret_t AlicSec; 
    dh_secret_t BobSec; 
    dh_exchange_t AlicExc; 
    dh_exchange_t BobExc; 


    dh_generate(AlicSec); 
    dh_encode(AlicExc, AlicSec, AlicePriv);

    assert(dh_generate(BobSec, &AlicExc)); 
    NNI BobsNum; 
    assert(dh_combine(BobsNum, AlicExc, BobSec, AlicePub));
    dh_encode(BobExc, BobSec, BobPriv); 

    NNI AlicNum;
    assert(dh_combine(AlicNum, BobExc, AlicSec, BobPub));
    std::cout << BobsNum << std::endl;
    std::cout << AlicNum << std::endl;
}