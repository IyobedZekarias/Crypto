#include "crypto.h"
#include <iostream>
#include <iomanip>
#include <cstring>

using hash_t = std::vector<uint64_t>; 
using namespace crypto; 

std::ostream & operator<<(std::ostream &out,const hash_t &buffer) {
    for (size_t i=0; i<buffer.size(); i++) {
        if (i%2 == 0)
            out << std::setw(4) << std::setfill(' ') << std::hex << i << ":";
        out << " " << std::setw(16) << std::setfill('0') << std::hex << +buffer[i];
        if (i%2 == 1)
            out << "\n";
    }
    if (buffer.size()%16)
        out << "\n";
    return out;
}

uint64_t rightRotate(uint64_t x, int d)
{
    /**
     * geeksforgeeks.com
     * By doing x >> d, we move the first(left most) d bits
     * to the right most d bits, and at the same time we move
     * the right most d bits to the right side,
     * performing OR operation between the two gives use the
     * required result.
     * */
 
    return (x >> d) | (x << (64 - d));
}

bool thash(const buffer_t &plain,hash_t &hash, hash_t h, int &t, bool mod){ 
    hash_t plain_h; 
    uint128_t s; 
    if(mod){ 
        //std::string SHA512V = "SHA-512/" + std::to_string(t);
        buffer_t SHA512V = {'S', 'H', 'A', '-', '5', '1', '2', '/'};
        char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
        int temp = t; 
        int hund = temp / 100;
        temp = temp % 100; 
        int tens = temp / 10; 
        temp = temp % 10; 
        int ones = temp; 
        SHA512V.push_back(digits[hund]); 
        SHA512V.push_back(digits[tens]); 
        SHA512V.push_back(digits[ones]); 
        const buffer_t ConstSHA512V = SHA512V; 
        bool additional = ConstSHA512V.size()%8; 
        const uint64_t *p = reinterpret_cast<const uint64_t*>(&ConstSHA512V[0]); 
        uint64_t n = static_cast<uint64_t>(ceil(ConstSHA512V.size() / sizeof(p[0]))); 
        for(int i = 0; i < n+additional; i++){
            plain_h.push_back(p[i]);
        }
        s = ConstSHA512V.size() * 8; 
    } else {
        bool additional = plain.size()%8; 
        const uint64_t *p = reinterpret_cast<const uint64_t*>(&plain[0]); 
        uint64_t n = static_cast<uint64_t>(ceil(plain.size() / sizeof(p[0]))); 
        for(int i = 0; i < n+additional; i++){
            plain_h.push_back(p[i]);
        }
        s = plain.size() * 8; 
    }
 
    for(auto i = plain_h.begin(); i != plain_h.end(); ++i){
        *i = __builtin_bswap64(*i);
    }
    
    if((plain_h.size())%16){
        if((s/8)%8) 
            plain_h[plain_h.size()-1] = plain_h[plain_h.size()-1] | (0x8000000000000000 >> ((s/8) * 8));
        else 
            plain_h.push_back(0x8000000000000000);
        if((plain_h.size() + 1) * 64 == 1024){
            plain_h.push_back(0x0000000000000000);
            plain_h.push_back(0x0000000000000000);
        }
        plain_h.resize(plain_h.size() + (16-((plain_h.size())%16))-2); 
        plain_h.push_back(static_cast<uint64_t>(s >> 64));  
        plain_h.push_back(s & 0xffffffffffffffff);
    }


    if(mod){
        for(auto i = h.begin(); i != h.end(); ++i){
            *i = *i ^ 0xa5a5a5a5a5a5a5a5; 
        }
    }
    
    
    hash_t k = {0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc, 0x3956c25bf348b538, 
                0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118, 0xd807aa98a3030242, 0x12835b0145706fbe, 
                0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2, 0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235, 
                0xc19bf174cf692694, 0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65, 
                0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5, 0x983e5152ee66dfab, 
                0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4, 0xc6e00bf33da88fc2, 0xd5a79147930aa725, 
                0x06ca6351e003826f, 0x142929670a0e6e70, 0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 
                0x53380d139d95b3df, 0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b, 
                0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30, 0xd192e819d6ef5218, 
                0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8, 0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 
                0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8, 0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373, 
                0x682e6ff3d6b2b8a3, 0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec, 
                0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b, 0xca273eceea26619c, 
                0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, 0x06f067aa72176fba, 0x0a637dc5a2c898a6, 
                0x113f9804bef90dae, 0x1b710b35131c471b, 0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc, 
                0x431d67c49c100d4c, 0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817};
    
    uint64_t limit = 0xffffffffffffffff;
    // hash_t test = {0b1001};
    // std::cout << test << std::endl;
    // test[0] = rightRotate(test[0], 1); 
    // std::cout << test << std::endl;
    int checker; 
    if(!(plain_h.size()/16)) checker = 1; 
    else checker = static_cast<int>(plain_h.size()/16); 
    for(int i = 0; i < checker; i++){
        hash_t w; 
        w.resize(80);
        int wi = 0; 
        for(int j = i * 16; j < (i*16 + 16); j++, wi++){
            if(plain_h.size()/16) w[wi] = plain_h[j];
            else w[wi] = 0x0000000000000000;
        }
        
        
        for(int j = 16; j < 80; j++){
            uint64_t s0 = (rightRotate(w[j-15], 1)) ^ (rightRotate(w[j-15], 8)) ^ (w[j-15] >> 7);
            uint64_t s1 = (rightRotate(w[j-2], 19)) ^ (rightRotate(w[j-2], 61)) ^ (w[j-2] >> 6);  
            w[j] = (w[j-16] + s0 + w[j-7] + s1) % limit; 
        }
        

        uint64_t a_wv = h[0]; 
        uint64_t b_wv = h[1]; 
        uint64_t c_wv = h[2]; 
        uint64_t d_wv = h[3]; 
        uint64_t e_wv = h[4]; 
        uint64_t f_wv = h[5]; 
        uint64_t g_wv = h[6]; 
        uint64_t h_wv = h[7]; 

        for(int j = 0; j < 80; j++){
            uint64_t S0 = (rightRotate(a_wv, 28)) ^ (rightRotate(a_wv, 34)) ^ (rightRotate(a_wv, 39)); 
            uint64_t S1 = (rightRotate(e_wv, 14)) ^ (rightRotate(e_wv, 18)) ^ (rightRotate(e_wv, 41));
            uint64_t ch = (e_wv & f_wv) ^ ((~e_wv) & g_wv); 
            uint64_t temp1 = (h_wv + S1 + ch + k[j] + w[j]) % limit; 
            uint64_t maj = (a_wv & b_wv) ^ (a_wv & c_wv) ^ (b_wv & c_wv); 
            uint64_t temp2 = (S0 + maj) % limit; 

            h_wv = g_wv; 
            g_wv = f_wv; 
            f_wv = e_wv; 
            e_wv = (d_wv + temp1) % limit; 
            d_wv = c_wv; 
            c_wv = b_wv; 
            b_wv = a_wv; 
            a_wv = (temp1 + temp2) % limit; 
        }

        h[0] = (h[0] + a_wv) % limit;
        h[1] = (h[1] + b_wv) % limit;
        h[2] = (h[2] + c_wv) % limit;
        h[3] = (h[3] + d_wv) % limit;
        h[4] = (h[4] + e_wv) % limit;
        h[5] = (h[5] + f_wv) % limit;
        h[6] = (h[6] + g_wv) % limit;
        h[7] = (h[7] + h_wv) % limit;
    } 

    if(mod && t){
        return thash(plain, hash, h, t, 0);
    } else if(t && !mod){
        int inverse = 512 - t; 
        for(auto i = h.rbegin(); i != h.rend(); ++i){
            if((inverse - 64) >= 0){
                h.erase(std::next(i).base());
                inverse -= 64;
            } else break;
        }
        t = inverse; 
        h[h.size()-1] = h[h.size()-1] & (limit << inverse); 
        hash = h; 
    } else {
        hash = h; 
        std::cout << h; 
    }

    return true;  
}


namespace crypto {
    bool hash_sha512(const buffer_t &plain,buffer_t &hash, int t){ 
        hash_t h = {0x6a09e667f3bcc908, 0xbb67ae8584caa73b, 0x3c6ef372fe94f82b, 0xa54ff53a5f1d36f1, 
                0x510e527fade682d1, 0x9b05688c2b3e6c1f, 0x1f83d9abfb41bd6b, 0x5be0cd19137e2179}; 
        hash_t rethash; 
        if(t){
            thash(plain, rethash, h, t, 1);
        }    
        else
            thash(plain, rethash, h, t, 0); 
        
        for(auto i = rethash.begin(); i != rethash.end(); ++i){
            *i = __builtin_bswap64(*i);
        }
        const hash_t ConstHash = rethash; 
        const uint8_t *p = reinterpret_cast<const uint8_t*>(&ConstHash[0]); 
        buffer_t temp(p, p+ConstHash.size()*8-(t/8));  
        hash = temp; 

        return true; 
    }
        
}