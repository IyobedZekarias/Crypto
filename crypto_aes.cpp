#include "crypto.h"
#include <iomanip>


#define DO_ENC_BLOCK(m,k) \
    do{\
        m = _mm_xor_si128       (m, k[ 0]); \
        m = _mm_aesenc_si128    (m, k[ 1]); \
        m = _mm_aesenc_si128    (m, k[ 2]); \
        m = _mm_aesenc_si128    (m, k[ 3]); \
        m = _mm_aesenc_si128    (m, k[ 4]); \
        m = _mm_aesenc_si128    (m, k[ 5]); \
        m = _mm_aesenc_si128    (m, k[ 6]); \
        m = _mm_aesenc_si128    (m, k[ 7]); \
        m = _mm_aesenc_si128    (m, k[ 8]); \
        m = _mm_aesenc_si128    (m, k[ 9]); \
        m = _mm_aesenclast_si128(m, k[10]);\
    }while(0)

#define DO_DEC_BLOCK(m,k) \
    do{\
        m = _mm_xor_si128       (m, k[10]); \
        m = _mm_aesdec_si128    (m, k[11]); \
        m = _mm_aesdec_si128    (m, k[12]); \
        m = _mm_aesdec_si128    (m, k[13]); \
        m = _mm_aesdec_si128    (m, k[14]); \
        m = _mm_aesdec_si128    (m, k[15]); \
        m = _mm_aesdec_si128    (m, k[16]); \
        m = _mm_aesdec_si128    (m, k[17]); \
        m = _mm_aesdec_si128    (m, k[18]); \
        m = _mm_aesdec_si128    (m, k[19]); \
        m = _mm_aesdeclast_si128(m, k[0]);\
    }while(0)

using namespace crypto; 
#define AES_128_key_exp(k, rcon) aes_128_key_expansion(k, _mm_aeskeygenassist_si128(k, rcon))

//Key Schedule Calc
static __m128i aes_128_key_expansion(__m128i key, __m128i keygened){
    keygened = _mm_shuffle_epi32(keygened, _MM_SHUFFLE(3,3,3,3));
    key = _mm_xor_si128(key, _mm_slli_si128(key, 4));
    key = _mm_xor_si128(key, _mm_slli_si128(key, 4));
    key = _mm_xor_si128(key, _mm_slli_si128(key, 4));
    return _mm_xor_si128(key, keygened);
}


void aes128_load_key_enc (const unsigned char *userkey, block_t *key_schedule) { 
    key_schedule[0] = _mm_loadu_si128(reinterpret_cast<const block_t*>(userkey));
    key_schedule[1]  = AES_128_key_exp(key_schedule[0], 0x01);
    key_schedule[2]  = AES_128_key_exp(key_schedule[1], 0x02);
    key_schedule[3]  = AES_128_key_exp(key_schedule[2], 0x04);
    key_schedule[4]  = AES_128_key_exp(key_schedule[3], 0x08);
    key_schedule[5]  = AES_128_key_exp(key_schedule[4], 0x10);
    key_schedule[6]  = AES_128_key_exp(key_schedule[5], 0x20);
    key_schedule[7]  = AES_128_key_exp(key_schedule[6], 0x40);
    key_schedule[8]  = AES_128_key_exp(key_schedule[7], 0x80);
    key_schedule[9]  = AES_128_key_exp(key_schedule[8], 0x1B);
    key_schedule[10] = AES_128_key_exp(key_schedule[9], 0x36);
} 


void aes128_load_key(const unsigned char *userkey, block_t *key_schedule) {
    aes128_load_key_enc(userkey, key_schedule); 
    key_schedule[11] = _mm_aesimc_si128(key_schedule[9]);
    key_schedule[12] = _mm_aesimc_si128(key_schedule[8]);
    key_schedule[13] = _mm_aesimc_si128(key_schedule[7]);
    key_schedule[14] = _mm_aesimc_si128(key_schedule[6]);
    key_schedule[15] = _mm_aesimc_si128(key_schedule[5]);
    key_schedule[16] = _mm_aesimc_si128(key_schedule[4]);
    key_schedule[17] = _mm_aesimc_si128(key_schedule[3]);
    key_schedule[18] = _mm_aesimc_si128(key_schedule[2]);
    key_schedule[19] = _mm_aesimc_si128(key_schedule[1]);
}   


namespace crypto {

    int Check_CPU_support_AES(){ 
        unsigned int a,b,c,d; 
        cpuid(1, a,b,c,d); 
        return (c & bit_AES); 
    }

    bool encode_aes128_ecb(const buffer_t &plain, buffer_t key, buffer_t &cipher){
        if(!Check_CPU_support_AES()) return false; 
        cipher.clear(); 
        unsigned char *user_key = &key[0]; 
        cipher.resize(plain.size()); 
        block_t key_schedule[20]; 
        aes128_load_key(user_key, key_schedule);

        buffer_t plain_b; 
        size_t l; 
        if(plain.size() % 16){
            l = plain.size() / 16 + 1; 
            cipher.resize(plain.size() + (16-(plain.size()%16)));  
            std::copy(plain.begin(), plain.end(), std::back_inserter(plain_b));
            plain_b.push_back(0x80); 
            plain_b.resize(plain.size() + (16-(plain.size()%16)), 0x00);  
        } else {
            l = plain.size()/16;
            plain_b = plain; 
        }        

        u_char cipher_c[cipher.size()];
        const u_char *plain_c = &plain_b[0];
        for(size_t i = 0; i < l; i++){
            block_t m = _mm_loadu_si128(&(reinterpret_cast<const block_t*>(plain_c))[i]); 
            DO_ENC_BLOCK(m, key_schedule); 
            _mm_storeu_si128(&(reinterpret_cast<block_t*>(cipher_c))[i], m); 
        }
        
        cipher.assign(cipher_c, cipher_c+plain_b.size()); 
        return true; 
    }

    bool decode_aes128_ecb(const buffer_t &cipher, buffer_t key,buffer_t &plain){
        if(!Check_CPU_support_AES()) return false; 
        plain.clear(); 
        unsigned char *user_key = &key[0]; 
        plain.resize(cipher.size()); 
        block_t key_schedule[20]; 
        aes128_load_key(user_key, key_schedule); 


        const u_char *cipher_c = &cipher[0];
        u_char plain_c[cipher.size()];
        for(size_t i = 0; i < (cipher.size()/16); i++){
            block_t m = _mm_loadu_si128(&(reinterpret_cast<const block_t*>(cipher_c))[i]); 
            DO_DEC_BLOCK(m, key_schedule); 
            _mm_storeu_si128(&(reinterpret_cast<block_t*>(plain_c))[i], m); 
        }

        plain.assign(plain_c, plain_c+cipher.size()); 

        for(auto i = plain.rbegin(); i != plain.rend(); ++i){
            if(*i == 0x00) 
                plain.erase((i + 1).base());  
            else if(*i == 0x80){
                plain.erase((i + 1).base()); 
                break;
            } else break;
        }   

        return true; 
    }

}