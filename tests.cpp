#include <cassert>
#include <stdio.h>
#include <iomanip>
#include "crypto.h"

using namespace crypto;
using namespace std;

bool compareBuffers(buffer_t f_buffer, buffer_t s_buffer, std::string test){
    if(f_buffer.size() == s_buffer.size()){
            for(struct {std::vector<u_char>::const_iterator i; std::vector<u_char>::const_iterator j;} s = {f_buffer.begin(), s_buffer.begin()}; 
                    s.i != f_buffer.end(); ++s.i, ++s.j){
                        if(*s.i != *s.j) {
                            std::cout << test << " FAILED:" << std::endl;
                            std::cout << f_buffer << std::endl;
                            std::cout << "and " << std::endl;
                            std::cout << s_buffer << std::endl;
                            std::cout << "are not equal" << std::endl; 
                            return false;
                        }
                    }
            std::cout << ".";
            return true; 
    } else {
        std::cout << std::uppercase << std::string(test) << " FAILED:" << std::endl;
        std::cout << f_buffer << std::endl;
        std::cout << "and " << std::endl;
        std::cout << s_buffer << std::endl;
        std::cout << "are not equal" << std::endl; 
        return false;
    }
}

int main()
{
    {
        std::cout << "RANDOM NUMBER TEST¯¯";
        buffer_t buffer; 
        int bytes = 4; 

        assert(urand(bytes, buffer));
        std::cout << ".";
        assert(rdrand(bytes, buffer));
        std::cout << ".";
        if(Check_CPU_support_RDSEED()){
            assert(rdseed(bytes, buffer)); 
            std::cout << ".";
        } 
        assert(randDev(bytes, buffer));
        std::cout << ".";
        assert(LCG(bytes, buffer));
        std::cout << ".";
        cout << "✓" << std::endl;
    }
    {
        std::cout << "XOR TEST¯¯";

        buffer_t key, in, out, inChecker; 
        urand(8, key); 
        urand(8, in); 

        assert(XOR(key, in, out));
        std::cout << ".";

        assert(XOR(key, out, inChecker)); 
        std::cout << ".";

        assert(compareBuffers(in, inChecker, "XOR"));
        std::cout << ".✓" << std::endl;
    }
    {
        std::cout << "AES TEST¯¯"; 
        buffer_t key = {0x54, 0x68, 0x61, 0x74, 0x73, 0x20, 0x6D, 0x79, 0x20, 0x4B, 0x75, 0x6E, 0x67, 0x20, 0x46, 0x75};
        buffer_t plaintext = {0x54, 0x77, 0x6F, 0X20, 0X4F, 0X6E, 0X65, 0X20, 0X4E, 0X69, 0X6E, 0X65, 0X20, 0X54, 0X77, 0X6F};
        buffer_t cipher = {0x29, 0xC3, 0X50, 0X5F, 0X57, 0X14, 0X20, 0XF6, 0X40, 0X22, 0X99, 0XB3, 0X1A, 0X02, 0XD7, 0X3A}; 
        buffer_t computed_cipher; 
        buffer_t computed_plain; 

        if(Check_CPU_support_AES()){
            assert(encode_aes128_ecb(plaintext, key, computed_cipher)); 
            std::cout << "."; 
            assert(compareBuffers(computed_cipher, cipher, "AES ENCRYPTION TEST")); 
            std::cout << "."; 
            assert(decode_aes128_ecb(cipher, key, computed_plain));
            std::cout << "."; 
            assert(compareBuffers(plaintext, computed_plain, "AES DECRYPTION TEST")); 
            std::cout << ".✓" << std::endl;
        }
    }


    return 0;

}




