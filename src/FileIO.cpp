#include "crypto.h"
#include <iomanip>

// NOTICE: there is NOT using namespace std here on purpose

namespace crypto {
    bool readFile(const char* filename, RSApublic &pub){
        std::string line;
        std::ifstream pubfile(filename, std::ios::in | std::ios::app); 
        if(!pubfile) return false; 
        if(!getline(pubfile, line)) return false; 
        *pub.e = NNI(line.c_str());
        if(!getline(pubfile, line)) return false; 
        *pub.n = NNI(line.c_str()); 
        getline(pubfile, line); 
        if(getline(pubfile, line)) return false; 
        return true; 
    }   
    
    bool readFile(const char* filename, RSAprivate &priv){
        std::string line;
        std::ifstream privfile(filename, std::ios::in | std::ios::app); 
        if(!privfile) return false;
        if(!getline(privfile, line)) return false; 
        *priv.n = NNI(line.c_str());
        if(!getline(privfile, line)) return false;
        *priv.p = NNI(line.c_str());
        if(!getline(privfile, line)) return false;
        *priv.q = NNI(line.c_str());
        if(!getline(privfile, line)) return false;
        *priv.d = NNI(line.c_str());
        if(!getline(privfile, line)) return false;
        *priv.phi = NNI(line.c_str());
        return true; 
    }

    bool writeFile(const char * filename, RSAprivate &priv){
        std::string s(filename); 
        if(s.substr(s.length()-4) != ".iyo") s.append(".iyo"); 
        std::ofstream rsapriv(s, std::ios::out | std::ios::app); 
        if(rsapriv){
            rsapriv << *priv.n;
            rsapriv << *priv.p;
            rsapriv << *priv.q;
            rsapriv << *priv.d;
            rsapriv << *priv.phi;
            return true; 
        } else return false;
    }

     bool writeFile(const char * filename, RSApublic &pub){
        std::string s(filename); 
        if(s.substr(s.length()-4) != ".iyo") s.append(".iyo"); 
        std::ofstream rsapub(s, std::ios::out | std::ios::app); 
        if(rsapub){
            rsapub << *pub.e;
            rsapub << *pub.n;
            return true; 
        } else return false;
    }

    bool readFile(const char* fileName, buffer_t &buffer){
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

    bool writeFile(const char* fileName, buffer_t buffer){
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