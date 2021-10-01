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
        assert(LCG(bytes, buffer));
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


    return 0;

}




