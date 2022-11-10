#include "../src/crypto_iz.h"

using namespace crypto;

int main(int argc, char** argv){
    if(argc != 3){
        std::cout << "Usage: " << std::string(argv[0]) << " <#bytes> <filename>" << std::endl; 
        return 0; 
    }

    MakeBinFile(argv[2], stoi(std::string(argv[1])));

}