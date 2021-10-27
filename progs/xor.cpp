#include "../src/crypto.h"
#include <stdint.h>
#include <fcntl.h>
#include <iomanip>

using namespace crypto;

int main(int argc, char** argv){
    if(argc != 4){
        std::cout << "Usage: " << std::string(argv[0]) << " <key> <input> <output>" << std::endl; 
    }

    //get some random binary digits
    buffer_t buffer; 
    urand(5, buffer); 

    //put random digits into a file
    int fd = open(argv[2], O_CREAT|O_TRUNC|O_WRONLY, 0666);
    write(fd, &(buffer[0]), buffer.size()*sizeof(buffer[0]));
    
    //run the xor 
    buffer_t key_v;
    ToBuffer(argv[1], key_v); 
    XOR(key_v, argv[2], argv[3]); 

    std::ifstream inFile (argv[2], std::ios::in | std::ios::binary);
    std::ifstream outFile (argv[3], std::ios::in | std::ios::binary);  

    if(inFile && outFile){
        std::vector<u_char> in; 
        std::vector<u_char> out; 

        inFile.seekg(0, inFile.end);
        long int filelength = inFile.tellg(); 
        inFile.seekg(0, inFile.beg); 
        in.resize(static_cast<size_t>(filelength));
        out.resize(static_cast<size_t>(filelength));

        if(!inFile.read(reinterpret_cast<char*>(&in[0]), filelength)) return 0; 
        if(!outFile.read(reinterpret_cast<char*>(&out[0]), filelength)) return 0; 

        inFile.close(); 
        outFile.close(); 
        
        std::cout << std::string(argv[2]) << std::endl;
        for(auto x : in){ std::cout << std::uppercase << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(x) << ' '; }
        std::cout << std::endl; 

        std::cout << std::string(argv[3]) << std::endl;
        for(auto x : out){ std::cout << std::uppercase << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(x) << ' '; }
        std::cout << std::endl; 
    }

}