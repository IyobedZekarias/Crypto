#include <cassert>
#include <stdio.h>
#include <iomanip>
#include "crypto.h"

using namespace crypto;
using namespace std;

bool compareBuffers(buffer_t f_buffer, buffer_t s_buffer, string test){
    if(f_buffer.size() == s_buffer.size()){
            for(struct {vector<u_char>::const_iterator i; vector<u_char>::const_iterator j;} s = {f_buffer.begin(), s_buffer.begin()}; 
                    s.i != f_buffer.end(); ++s.i, ++s.j){
                        if(*s.i != *s.j) {
                            cout << "\n" << setw(32) << setfill(' ') << uppercase << string(test) << " FAILED:" << endl;
                            cout << f_buffer << endl;
                            cout << "and " << endl;
                            cout << s_buffer << endl;
                            cout << "are not equal at positon" << endl; 
                            cout << hex << static_cast<int>(*s.i) << " " << static_cast<int>(*s.j) << endl;
                            return false;
                        }
                    }
            cout << ".";
            return true; 
    } else {
        cout << "\n" << setw(32) << setfill(' ') << uppercase << string(test) << " FAILED:" << endl;
        cout << f_buffer << endl;
        cout << "and " << endl;
        cout << s_buffer << endl;
        cout << "are not equal" << endl; 
        return false;
    }
}

int main()
{
    {
        cout << "RANDOM NUMBER TEST¯¯";
        buffer_t buffer; 
        int bytes = 4; 

        assert(urand(bytes, buffer));
        cout << ".";
        if(Check_CPU_support_RDRAND())
            assert(rdrand(bytes, buffer));
        cout << ".";
        if(Check_CPU_support_RDSEED())
            assert(rdseed(bytes, buffer)); 
        cout << ".";
        assert(randDev(bytes, buffer));
        cout << ".";
        // assert(LCG(bytes, buffer));
        cout << ".";
        cout << setw(5) << setfill('.') << "✅";
        if(!Check_CPU_support_RDSEED()) cout << "-SYSTEM DOES NOT SUPPORT RDSEED"; 
        if(!Check_CPU_support_RDRAND()) cout << "-SYSTEM DOES NOT SUPPORT RDRAND";
        cout << endl;
    }
    {
        cout << "XOR TEST¯¯";

        buffer_t key, in, out, inChecker; 
        urand(8, key); 
        urand(8, in); 

        assert(XOR(key, in, out));
        cout << ".";

        assert(XOR(key, out, inChecker)); 
        cout << ".";

        assert(compareBuffers(in, inChecker, "XOR"));
        cout << setw(17) << setfill('.') << ".✅" << endl;
    }
    {
        cout << "AES ECB TEST¯¯"; 
        if(Check_CPU_support_AES()){
            buffer_t key = {0x54, 0x68, 0x61, 0x74, 0x73, 0x20, 0x6D, 0x79, 0x20, 0x4B, 0x75, 0x6E, 0x67, 0x20, 0x46, 0x75};
            buffer_t plaintext = {0x54, 0x77, 0x6F, 0X20, 0X4F, 0X6E, 0X65, 0X20, 0X4E, 0X69, 0X6E, 0X65, 0X20, 0X54, 0X77, 0X6F};
            buffer_t cipher = {0x29, 0xC3, 0X50, 0X5F, 0X57, 0X14, 0X20, 0XF6, 0X40, 0X22, 0X99, 0XB3, 0X1A, 0X02, 0XD7, 0X3A, 0xB3, 0XE4, 0X6F, 0X11, 0XBA, 0X8D, 0X2B, 0X97, 0XC1, 0X87, 0X69, 0X44, 0X9A, 0X89, 0XE8, 0X68}; 
            buffer_t computed_cipher; 
            buffer_t computed_plain; 

            assert(encode_aes128_ecb(plaintext, key, computed_cipher)); 
            cout << "."; 
            assert(compareBuffers(computed_cipher, cipher, "AES ECB ENCRYPTION TEST")); 
            cout << "."; 
            assert(decode_aes128_ecb(cipher, key, computed_plain));
            cout << "."; 
            assert(compareBuffers(plaintext, computed_plain, "AES ECB DECRYPTION TEST")); 
            cout << setw(11) << setfill('.') << ".✅" << endl;
        } else cout << setw(20) << setfill('.') << "❌" << "-CPU DOES NOT SUPPORT AES" << endl;
    }
    {
        cout << "AES CBC TEST¯¯"; 
        if(Check_CPU_support_AES()){
            buffer_t key = {0XFD, 0XD9, 0X4D, 0X36, 0X96, 0X9A, 0X2B, 0X66, 0X97, 0X69, 0X59, 0X45, 0XC3, 0XE8, 0X91, 0X26};
            buffer_t plaintext = {0XF0, 0X5E, 0XA3, 0XB6, 0X1D, 0X9A, 0X15, 0X0E, 0X52, 0XCF, 0X85, 0X96, 0X16, 0XAC, 0X68, 0X40};
            buffer_t cipher = {0X53, 0X1E, 0XD4, 0XA3, 0X2F, 0XE8, 0X73, 0X2D, 0X30, 0X5E, 0X9C, 0X24, 0XC4, 0XF4, 0XF9, 0X40, 0XDA, 0X5D, 0X9E, 0XC2, 0XD0, 0X5B, 0X0F, 0XE8, 0X7B, 0X39, 0X4D, 0XF0, 0XAC, 0X3C, 0XCE, 0XA8}; 
            buffer_t IV = {0XCF, 0X2C, 0X83, 0X27, 0X51, 0X44, 0X63, 0X56, 0X83, 0X28, 0X02, 0X42, 0X6B, 0X85, 0X97, 0XFD}; 
            buffer_t computed_cipher; 
            buffer_t computed_plain; 

            assert(encode_aes128_cbc(plaintext, key, computed_cipher, IV)); 
            cout << "."; 
            assert(compareBuffers(computed_cipher, cipher, "AES CBC ENCRYPTION TEST")); 
            cout << "."; 
            assert(decode_aes128_cbc(cipher, key, computed_plain, IV));
            cout << "."; 
            assert(compareBuffers(plaintext, computed_plain, "AES CBC DECRYPTION TEST")); 
            cout << setw(11) << setfill('.') << ".✅" << endl;
        } else cout << setw(20) << setfill('.') << "❌" << "-CPU DOES NOT SUPPORT AES" << endl;
    }
    {
        cout << "SHA-512--"; 
        buffer_t plaintext = {0x61, 0x62, 0x63}; 
        buffer_t Writtenhash = {0xdd, 0xaf, 0x35, 0xa1, 0x93, 0x61, 0x7a, 0xba, 0xcc, 0x41, 0x73, 0x49, 0xae, 0x20, 0x41, 0x31, 0x12, 0xe6,
                                0xfa, 0x4e, 0x89, 0xa9, 0x7e, 0xa2, 0x0a, 0x9e, 0xee, 0xe6, 0x4b, 0x55, 0xd3, 0x9a, 0x21, 0x92, 0x99, 0x2a,
                                0x27, 0x4f, 0xc1, 0xa8, 0x36, 0xba, 0x3c, 0x23, 0xa3, 0xfe, 0xeb, 0xbd, 0x45, 0x4d, 0x44, 0x23, 0x64, 0x3c, 
                                0xe8, 0x0e, 0x2a, 0x9a, 0xc9, 0x4f, 0xa5, 0x4c, 0xa4, 0x9f};
        buffer_t computedHash; 
        assert(hash_sha512(plaintext, computedHash, 0));
        cout << ".";
        assert(compareBuffers(computedHash, Writtenhash, "SHA512 HASHING TEST"));
        cout << setw(19) << setfill('.') << ".✅" << endl;
    }
    {
        cout << "SHA-512/256--"; 
        buffer_t plaintext(112); 
        const std::vector<uint64_t> plaintext64 = {__builtin_bswap64(0x6162636465666768), __builtin_bswap64(0x6263646566676869), __builtin_bswap64(0x636465666768696A), __builtin_bswap64(0x6465666768696A6B),
                                             __builtin_bswap64(0x65666768696A6B6C), __builtin_bswap64(0x666768696A6B6C6D), __builtin_bswap64(0x6768696A6B6C6D6E), __builtin_bswap64(0x68696A6B6C6D6E6F),
                                             __builtin_bswap64(0x696A6B6C6D6E6F70), __builtin_bswap64(0x6A6B6C6D6E6F7071), __builtin_bswap64(0x6B6C6D6E6F707172), __builtin_bswap64(0x6C6D6E6F70717273),
                                             __builtin_bswap64(0x6D6E6F7071727374), __builtin_bswap64(0x6E6F707172737475)};
        const uint8_t *p = reinterpret_cast<const uint8_t*>(&plaintext64[0]); 
        uint8_t n = static_cast<uint8_t>(plaintext64.size() * 8); 
        std::copy(p, p+n, plaintext.begin()); 
        buffer_t Writtenhash = {0x39, 0x28, 0xE1, 0x84, 0xFB, 0x86, 0x90, 0xF8, 0x40, 0xDA, 
                                0x39, 0x88, 0x12, 0x1D, 0x31, 0xBE, 0x65, 0xCB, 0x9D, 0x3E, 
                                0xF8, 0x3E, 0xE6, 0x14, 0x6F, 0xEA, 0xC8, 0x61, 0xE1, 0x9B, 0x56, 0x3A};
        buffer_t computedHash; 
        assert(hash_sha512(plaintext, computedHash, 256));
        cout << ".";
        assert(compareBuffers(computedHash, Writtenhash, "SHA512 HASHING TEST"));
        cout << setw(15) << setfill('.') << ".✅" << endl;
    }


    return 0;

}




