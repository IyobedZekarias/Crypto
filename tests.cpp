#include <cassert>
#include <stdio.h>
#include <iomanip>
#include "crypto.h"

using namespace crypto;
using namespace std;

bool XORchecker(char fileName[], std::vector<u_char> &fill){
    std::ifstream file (fileName, std::ios::in | std::ios::binary);
    if(file){
            std::cout << "open " << std::string(fileName) << " file check successful" << std::endl;
            file.seekg(0, file.end);
            long int filelength = file.tellg(); 
            file.seekg(0, file.beg); 
            fill.resize(static_cast<size_t>(filelength));

            if(!file.read(reinterpret_cast<char*>(&fill[0]), filelength)) return false; 
            std::cout << "file " << std::string(fileName) << " read successful" << std::endl;
            file.close();  
            
            std::cout << "Contents of " << std::string(fileName) << std::endl;
            for(auto x : fill){ std::cout << std::uppercase << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(x) << ' '; }
            std::cout << std::endl << std::endl; 

            return true;

    } else {std::cout << "opening " << std::string(fileName) << " file unsuccessful" << std::endl; return false;}
}



int main()
{

    std::cout << "STARTING RANDOM NUMBER TEST\n¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯" <<std::endl;
    buffer_t buffer; 
    int bytes = 4; 
    if(!urand(bytes, buffer)) std::cout << "urand failed" <<std::endl;
    else if(!rdrand(bytes, buffer)) std::cout << "rdrand failed" <<std::endl;
    //else if(!rdseed(bytes, buffer)) std::cout << "rdseed failed" <<std::endl;
    else if(!randDev(bytes, buffer)) std::cout << "randdev failed" <<std::endl;
    else if(!LCG(bytes, buffer)) std::cout << "LCG failed" <<std::endl;
    else cout << "all random tests passed" << std::endl << std::endl;

    std::cout << "STARTING XOR TEST\n¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯" <<std::endl;
    char key[] = "00FFAFF";
    char in[] = "in_test.bin";
    char out[] = "out_test.bin";
    char in_checker[] = "in_test_checker.bin"; 

    std::cout << "Key for testing xor is: " << std::string(key) << std::endl << std::endl; 

    std::vector<u_char> in_v, out_v, inChecker_v; 

    assert(XORchecker(in, in_v));

    assert(XOR(key, in, out));
    std::cout << "file " << std::string(out) << " creation and xor successful" << std::endl;

    assert(XORchecker(out, out_v));

    assert(XOR(key, out, in_checker)); 
    std::cout << "file " << std::string(in_checker) << " creation and xor successful" << std::endl;

    assert(XORchecker(in_checker, inChecker_v)); 

    if(in_v.size() == inChecker_v.size()){
        std::cout << std::string(in) << "     " << std::string(in_checker) << std::endl;
        for(struct {std::vector<u_char>::const_iterator i; std::vector<u_char>::const_iterator j;} s = {in_v.begin(), inChecker_v.begin()}; 
                s.i != in_v.end(); ++s.i, ++s.j){
                    if(*s.i != *s.j) {std::cout << "XOR failed " << std::string(in) << " and " << std::string(in_checker) << "are not equal" << std::endl; return 0;}
                    else std::cout << std::uppercase << std::setw(2) << std::setfill('0') << std::hex << "    " << static_cast<int>(*s.i) << "                  " << static_cast<int>(*s.j) << std::endl;
                }
        std::cout << "Conents of the two files match!!" << std::endl;

    } else std::cout << "XOR failed " << std::string(in) << " and " << std::string(in_checker) << "are not equal" << std::endl;

    return 0;




}




