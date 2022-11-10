#include "crypto_iz.h"

using namespace crypto; 

std::ostream & operator<<(std::ostream &out,const crypto::dh_secret_t &pub){
    out << "a: " << pub.a << "\n"; 
    out << "g: " << pub.g << "\n"; 
    out << "p: " << pub.p << "\n"; 
    return out; 
}

std::ostream & operator<<(std::ostream &out,const crypto::dh_exchange_t &priv){
    out << "p: " << priv.p << "\n"; 
    out << "g: " << priv.g << "\n";
    out << "half: " << priv.half << "\n";
    out << "sig: " << priv.sig << "\n"; 
    return out; 
}


namespace crypto {
    // create a new half of a DH secret, if e is given check it and be compatible with it (else fail)
    bool dh_generate(dh_secret_t &secret, const dh_exchange_t *e /*nullptr*/){
        if(e)
            secret.p = e->p; 
        else{
            secret.p.randnni(2); 
            if(secret.p % NNI(2) == 0) secret.p = secret.p + NNI(1); 
            int counter = 0;
            for(;;){
                if(Miller(secret.p, 40)){
                    break;
                    
                } else {
                    if(counter % 10 == 0) std::cout << "." << std::flush;
                    secret.p = secret.p + NNI(2); 
                    counter++; 
                    
                    }
            }
        }
        std::cout << std::endl;
        secret.a.randnni(32); 
        secret.g = NNI(3); 
        return true; 
    }

    // using half a DH secret and our own private key, make an encoded half
    bool dh_encode(dh_exchange_t &e,const dh_secret_t &secret,const RSAprivate &key){
        e.p = secret.p; 
        e.g = secret.g; 
        e.half = modexp(e.g, secret.a, secret.p); 
        buffer_t hash; 
        if(!hash_sha512(e.half.toBuffer(), hash, 256)) return false; 
        buffer_t mes; 
        decode_rsa(mes, hash, key); 
        e.sig = NNI(mes); 
        return true; 
    }

    // verify an encode half and combine with our secret to create the DH shared secret
    bool dh_combine(NNI &shared_secret,const dh_exchange_t &e,const dh_secret_t &secret,const RSApublic &pub){
        buffer_t hash, mes; 
        if(!hash_sha512(e.half.toBuffer(), hash, 256)) return false;
        encode_rsa(e.sig.toBuffer(), mes, pub);
        NNI s = NNI(mes);  
        if(s != NNI(hash)) return false; 
        
        shared_secret = modexp(e.half, secret.a, e.p); 
        return true; 
    }

}