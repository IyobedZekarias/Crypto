#include "../src/crypto_iz.h"
#include <stdint.h>
#include <cstring>

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

int main(int argc, char** argv)
{
    if(argc < 3){
        cout << "Usage: " << std::string(argv[0]) << " <source file> <-e | -d | -key> [destination file] [pub=RSApublic.iyo] [priv=RSAprivate.iyo]" << endl;
        return 0; 
    }

    RSAprivate priv; 
    RSApublic pub; 

    if(std::string(argv[2]) == "-e"){
        std::string s = "na"; 
        for(int i = 3; i < argc; i++){
            if(std::string(argv[i]).substr(0, 3) == "pub"){
                int eqloc = std::string(argv[i]).find('='); 
                s = std::string(argv[i]).substr(eqloc+1);
                break;
            }
        }
        if(s == "na"){
            remove("RSAprivate.iyo"); 
            remove("RSApublic.iyo");
            generate_rsa(&priv, &pub); 
            std::ofstream rsapriv("RSAprivate.iyo", std::ios::out | std::ios::app); 
            if(rsapriv){
                rsapriv << *priv.n;
                rsapriv << *priv.p;
                rsapriv << *priv.q;
                rsapriv << *priv.d;
                rsapriv << *priv.phi;
            }
            std::ofstream rsapub("RSApublic.iyo", std::ios::out); 
            if(rsapub){
                rsapub << *pub.e;
                rsapub << *pub.n;
            }
            rsapriv.close(); 
            rsapub.close(); 

        } else {
            std::string line;
            std::ifstream pubfile(s, std::ios::in | std::ios::app); 
            getline(pubfile, line); 
            *pub.e = NNI(line.c_str());
            getline(pubfile, line); 
            *pub.n = NNI(line.c_str());   
        }

        buffer_t mes, cip; 
        readFile(argv[1], mes); 
        encode_rsa(mes, cip, pub); 
        std::string d = "na"; 
        for(int i = 3; i < argc; i++){
            if(std::string(argv[i]).substr(0, 3) != "pub" && std::string(argv[i]).substr(0, 4) != "priv"){ 
                d = std::string(argv[i]);
                break;
            }
        }


        if(d == "na"){
            d = "RSAcipher"; 
            remove(d.c_str()); 
        }
        char * d2 = (char*)malloc(sizeof(char) * d.length());
        strcpy(d2, d.c_str()); 
        writeFile(d2, cip); 
        return 1; 
    }
    else if(std::string(argv[2]) == "-d"){
        std::string s = "na"; 
        for(int i = 3; i < argc; i++){
            if(std::string(argv[i]).substr(0, 4) == "priv"){
                int eqloc = std::string(argv[i]).find('='); 
                s = std::string(argv[i]).substr(eqloc+1);
                break;
            }
        }
        if(s == "na"){
            remove("RSAprivate.iyo"); 
            remove("RSApublic.iyo");
            generate_rsa(&priv, &pub); 
            std::ofstream rsapriv("RSAprivate.iyo", std::ios::out | std::ios::app); 
            if(rsapriv){
                rsapriv << *priv.n;
                rsapriv << *priv.p;
                rsapriv << *priv.q;
                rsapriv << *priv.d;
                rsapriv << *priv.phi;
            }
            std::ofstream rsapub("RSApublic.iyo", std::ios::out); 
            if(rsapub){
                rsapub << *pub.e;
                rsapub << *pub.n;
            }
            rsapriv.close(); 
            rsapub.close(); 

        } else {
            std::string line;
            std::ifstream pubfile(s, std::ios::in | std::ios::app); 
            getline(pubfile, line); 
            *priv.n = NNI(line.c_str());
            getline(pubfile, line); 
            *priv.p = NNI(line.c_str());
            getline(pubfile, line); 
            *priv.q = NNI(line.c_str());
            getline(pubfile, line); 
            *priv.d = NNI(line.c_str());
            getline(pubfile, line); 
            *priv.phi = NNI(line.c_str());
        }

        buffer_t mes, cip; 
        readFile(argv[1], cip); 
        decode_rsa(mes, cip, priv); 
        std::string d = "na"; 
        for(int i = 3; i < argc; i++){
            if(std::string(argv[i]).substr(0, 3) != "pub" && std::string(argv[i]).substr(0, 4) != "priv"){ 
                d = std::string(argv[i]);
                break;
            }
        }


        if(d == "na"){
            d = "RSAmessage"; 
            remove(d.c_str()); 
        }
        char * d2 = (char*)malloc(sizeof(char) * d.length());
        strcpy(d2, d.c_str()); 
        writeFile(d2, mes); 
        return 1; 
    } else if(std::string(argv[2]) == "-key" || std::string(argv[2]) == "-k"){
        if(argc < 4){
            cout << "Usage: " << std::string(argv[0]) << " <public key name> <-key> <private key name>" << endl;
            return 0;
        }
        generate_rsa(&priv, &pub);
        std::string s(argv[3]); 
        s.append(".iyo");  
        std::ofstream rsapriv(s, std::ios::out | std::ios::app); 
        if(rsapriv){
            rsapriv << *priv.n;
            rsapriv << *priv.p;
            rsapriv << *priv.q;
            rsapriv << *priv.d;
            rsapriv << *priv.phi;
        }
        s = std::string(argv[1]); 
        s.append(".iyo");
        std::ofstream rsapub(s, std::ios::out); 
        if(rsapub){
            rsapub << *pub.e;
            rsapub << *pub.n;
        }
        rsapriv.close(); 
        rsapub.close(); 
        
    } else {
        cout << "you must enter a valid choice, either: \n    -e: encrypt a file\n    -d: decrypt a file\n    -key: generate a public and private keypair\n"; 
    }


    
}
