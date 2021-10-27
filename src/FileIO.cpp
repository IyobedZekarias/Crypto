#include "crypto.h"
#include <iomanip>

// NOTICE: there is NOT using namespace std here on purpose

namespace crypto {
    bool readFile(char* fileName, buffer_t &buffer){
        std::ifstream inFile (fileName, std::ios::in | std::ios::binary); 
        if(inFile){
            inFile.seekg(0, inFile.end);
            long int filelength = inFile.tellg(); 
            inFile.seekg(0, inFile.beg); 
            buffer.resize(static_cast<size_t>(filelength));
            if(!inFile.read(reinterpret_cast<char*>(&buffer[0]), filelength)) return false; 

            inFile.close();
            return true; 
        } else return false;
    }

    bool writeFile(char* fileName, buffer_t buffer){
        std::ofstream outFile(fileName, std::ios::out | std::ios::binary); 
        if(outFile){
            if(!outFile.write(reinterpret_cast<char*>(&buffer[0]), sizeof(buffer[0])*buffer.size())) return false; 
            outFile.close(); 
            return true; 
        } else return false; 
    }

    void ToBuffer(char* array, buffer_t &buffer){
        std::string key_str(array); 
        for( size_t i = 0; i < key_str.length(); i+=2){
            u_char byte; 
            std::stringstream ss; 
            ss << key_str[i] << key_str[i+1]; 
            byte = static_cast<u_char>(std::stoul(ss.str(), nullptr, 16)); 
            buffer.push_back(byte); 
        }
    }

    void MakeBinFile(char * fileName, size_t bytes){
        buffer_t buffer; 
        rdrand(bytes, buffer); 

        writeFile(fileName, buffer); 
    }
}