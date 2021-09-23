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
                            cout << "are not equal" << endl; 
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
        cout << "AES TEST¯¯"; 
        if(Check_CPU_support_AES()){
            buffer_t key = {0x54, 0x68, 0x61, 0x74, 0x73, 0x20, 0x6D, 0x79, 0x20, 0x4B, 0x75, 0x6E, 0x67, 0x20, 0x46, 0x75};
            buffer_t plaintext = {0x54, 0x77, 0x6F, 0X20, 0X4F, 0X6E, 0X65, 0X20, 0X4E, 0X69, 0X6E, 0X65, 0X20, 0X54, 0X77, 0X6F};
            buffer_t cipher = {0x29, 0xC3, 0X50, 0X5F, 0X57, 0X14, 0X20, 0XF6, 0X40, 0X22, 0X99, 0XB3, 0X1A, 0X02, 0XD7, 0X3A, 0xB3, 0XE4, 0X6F, 0X11, 0XBA, 0X8D, 0X2B, 0X97, 0XC1, 0X87, 0X69, 0X44, 0X9A, 0X89, 0XE8, 0X68}; 
            buffer_t computed_cipher; 
            buffer_t computed_plain; 

            assert(encode_aes128_ecb(plaintext, key, computed_cipher)); 
            cout << "."; 
            assert(compareBuffers(computed_cipher, cipher, "AES ENCRYPTION TEST")); 
            cout << "."; 
            assert(decode_aes128_ecb(cipher, key, computed_plain));
            cout << "."; 
            assert(compareBuffers(plaintext, computed_plain, "AES DECRYPTION TEST")); 
            cout << setw(15) << setfill('.') << ".✅" << endl;
        } else cout << setw(20) << setfill('.') << "❌" << "-CPU DOES NOT SUPPORT AES" << endl;
    }


    return 0;

}




