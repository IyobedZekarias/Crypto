#include "../src/crypto_iz.h"
#include <stdint.h>

using namespace crypto;
using namespace std;

int main(int argc, char** argv){
    if(argc < 2){
        std::cout << "Usage: " << "\n"; 
        std::cout << "  gen dh_secret: " << std::string(argv[0]) << " -gs [sec=DHSec.zek] [exc=DHExchange.zek]\n"; 
        std::cout << "  gen dh_exchange: " << std::string(argv[0]) << " -ge <sec=DHSec.zek> <priv=RSAprivate.iyo> [exc=DHExchange.zek]\n"; 
        std::cout << "  gen secret number: " << std::string(argv[0]) << " -s <exc=DHExchange.zek> <sec=DHESec.zek> <pub=RSApublic.iyo> [secretNum]\n"; 
        std::cout << "  manual: " << std::string(argv[0]) << " man\n"; 
        return 0; 
    }

    if(std::string(argv[1]) == "-gs"){
        dh_secret_t sec; 
        dh_exchange_t exc; 
        std::string s = "na"; 
        for(int i = 1; i < argc; i++){
            if(std::string(argv[i]).substr(0, 3) == "sec"){
                int eqloc = std::string(argv[i]).find('='); 
                s = std::string(argv[i]).substr(eqloc+1);
                break;
            }
        }
        std::string t = "na"; 
        for(int i = 1; i < argc; i++){
            if(std::string(argv[i]).substr(0, 3) == "exc"){
                int eqloc = std::string(argv[i]).find('='); 
                t = std::string(argv[i]).substr(eqloc+1);
                break;
            }
        }

        if(s == "na") remove("DHSec.zek"); 
        else remove(s.c_str()); 
            
        if(t == "na") dh_generate(sec); 
        else{
            readFile(t.c_str(), exc); 
            dh_generate(sec, &exc); 
        }
        
        if(s == "na") writeFile("DHSec.zek", sec); 
        else writeFile(s.c_str(), sec); 
        

        return 1; 

    } else if(std::string(argv[1]) == "-ge"){
        RSAprivate priv; 
        dh_secret_t sec; 
        dh_exchange_t exc; 
        std::string secfile = "na"; 
        for(int i = 1; i < argc; i++){
            if(std::string(argv[i]).substr(0, 3) == "sec"){
                int eqloc = std::string(argv[i]).find('='); 
                secfile = std::string(argv[i]).substr(eqloc+1);
                break;
            }
        }

        std::string rsafile = "na"; 
        for(int i = 1; i < argc; i++){
            if(std::string(argv[i]).substr(0, 4) == "priv"){
                int eqloc = std::string(argv[i]).find('='); 
                rsafile = std::string(argv[i]).substr(eqloc+1);
                break;
            }
        }

        std::string excfile = "na"; 
        for(int i = 1; i < argc; i++){
            if(std::string(argv[i]).substr(0, 3) == "exc"){
                int eqloc = std::string(argv[i]).find('='); 
                excfile = std::string(argv[i]).substr(eqloc+1);
                break;
            }
        }

        if(rsafile == "na" || secfile=="na") {std::cout << "must include required files\n run ./dh man for more information\n"; return 0;}

        if(excfile == "na"){
            remove("DHExchange.zek");
            readFile(secfile.c_str(), sec); 
            readFile(rsafile.c_str(), priv); 
            dh_encode(exc, sec, priv);  
            writeFile("DHExchange.zek", exc); 
        } else {
            remove(excfile.c_str()); 
            readFile(secfile.c_str(), sec); 
            readFile(rsafile.c_str(), priv); 
            dh_encode(exc, sec, priv); 
            writeFile(excfile.c_str(), exc); 
        }

        return 1; 

    }
    else if(std::string(argv[1]) == "-s"){
        RSApublic pub; 
        dh_exchange_t exc; 
        dh_secret_t sec; 

        std::string secfile = "na"; 
        for(int i = 1; i < argc; i++){
            if(std::string(argv[i]).substr(0, 3) == "sec"){
                int eqloc = std::string(argv[i]).find('='); 
                secfile = std::string(argv[i]).substr(eqloc+1);
                break;
            }
        }

        std::string excfile = "na"; 
        for(int i = 1; i < argc; i++){
            if(std::string(argv[i]).substr(0, 3) == "exc"){
                int eqloc = std::string(argv[i]).find('='); 
                excfile = std::string(argv[i]).substr(eqloc+1);
                break;
            }
        }

        std::string pubfile = "na"; 
        for(int i = 1; i < argc; i++){
            if(std::string(argv[i]).substr(0, 3) == "pub"){
                int eqloc = std::string(argv[i]).find('='); 
                pubfile = std::string(argv[i]).substr(eqloc+1);
                break;
            }
        }

        std::string numfile = "na"; 
        for(int i = 1; i < argc; i++){
            if(std::string(argv[i]).substr(0, 3) != "pub" && std::string(argv[i]).substr(0, 3) != "exc" && std::string(argv[i]).substr(0, 3) != "sec" && std::string(argv[i]) != "-s"){
                int eqloc = std::string(argv[i]).find('='); 
                numfile = std::string(argv[i]).substr(eqloc+1);
                break;
            }
        }

        if(secfile == "na" || excfile == "na" || pubfile == "na"){std::cout << "must include required files\n run ./dh man for more information\n"; return 0;}

        NNI num; 
        readFile(excfile.c_str(), exc); 
        readFile(secfile.c_str(), sec); 
        readFile(pubfile.c_str(), pub); 
        dh_combine(num, exc, sec, pub); 

        if(numfile == "na"){
            remove("secretNumber"); 
            writeFile("secretNumber", num.toBuffer()); 
        } else {
            remove(numfile.c_str()); 
            writeFile(numfile.c_str(), num.toBuffer()); 
        }
        return 1; 
    }
    else if(std::string(argv[1]) == "man"){
        std::cout << "This program implements Diffie Hellman key exchange\n There are a few functionalities of this comand line tool\n";
        std::cout << "  1. The first option is generating a dh_secret with -gs flag\n";
        std::cout << "      This will take two optional files \'sec=\' and \'exc=\'\n";
        std::cout << "          sec: destination file for secret to be put in, if not given file will be created (optional)\n";
        std::cout << "          exc: dh_exchange file that was sent to you, if one has been sent your dh_secret\n";
        std::cout << "              will be made in compliance with there's (optional) \n\n"; 
        std::cout << "  2. The second option is generating a dh_exchange file with -ge flag\n";
        std::cout << "      This option has two required arguments and one optional file: \'sec=\', \'priv=\', and \'exc=\'\n"; 
        std::cout << "          sec: file that contains your secret dh_secret (required)\n";
        std::cout << "          priv: your private RSAkey (required)\n"; 
        std::cout << "          exc: destination file that has for dh_exchange generated if not included file will be created (optional)\n\n";
        std::cout << "  3. The third option is to generate the secret number between two people.\n"; 
        std::cout << "      This option has three required files and one optional file \'exc=\', \'sec=\', \'pub=\', and secretNumber file\n"; 
        std::cout << "          exc: exchange file from other party in key exchange (required)\n"; 
        std::cout << "          sec: your dh_secret file (required)\n"; 
        std::cout << "          pub: RSA public key of the other party (required)\n"; 
        std::cout << "          secretNumber: destination file for your secret number\n\n";
        return 1;      
    } else return 0;  

//     RSAprivate AlicePriv; 
//     RSApublic AlicePub; 
//     RSAprivate BobPriv; 
//     RSApublic BobPub; 

//     readFile("Apriv.iyo", AlicePriv); 
//     readFile("Apub.iyo", AlicePub); 
//     readFile("Bpriv.iyo", BobPriv); 
//     readFile("Bpub.iyo", BobPub); 

//     dh_secret_t AlicSec; 
//     dh_secret_t BobSec; 
//     dh_exchange_t AlicExc; 
//     dh_exchange_t BobExc; 


//     dh_generate(AlicSec); 
//     dh_encode(AlicExc, AlicSec, AlicePriv);

//     assert(dh_generate(BobSec, &AlicExc)); 
//     NNI BobsNum; 
//     assert(dh_combine(BobsNum, AlicExc, BobSec, AlicePub));
//     dh_encode(BobExc, BobSec, BobPriv); 

//     NNI AlicNum;
//     assert(dh_combine(AlicNum, BobExc, AlicSec, BobPub));
//     std::cout << BobsNum << std::endl;
//     std::cout << AlicNum << std::endl;
}