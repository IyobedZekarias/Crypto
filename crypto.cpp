#include "crypto.h"
#include <iomanip>

// NOTICE: there is NOT using namespace std here on purpose

std::ostream & operator<<(std::ostream &out,const crypto::buffer_t &buffer) {
    for (size_t i=0; i<buffer.size(); i++) {
        if (i%16 == 0)
            out << std::setw(4) << std::setfill(' ') << std::hex << i << ":";
        out << " " << std::setw(2) << std::setfill('0') << std::hex << +buffer[i];
        if (i%16 == 15)
            out << "\n";
    }
    if (buffer.size()%16)
        out << "\n";
    return out;
}

namespace crypto {

   //Accessing urandom and copying from file into buffer
   bool urand(size_t bytes,buffer_t &buffer){
        std::ifstream urandom("/dev/urandom", std::ios::in|std::ios::binary);
        if(urandom){
            std::copy_n(std::istream_iterator<uint8_t>(urandom), bytes, std::back_inserter(buffer)); 
            urandom.close(); 
            return true; 
        }
        urandom.close(); 
        return false; 
   }
   
   //using rdrand and copying number into buffer
   bool rdrand(size_t bytes, buffer_t &buffer){
        int high = 64, mid = 32, low = 16, bits = static_cast<int>(bytes) * 8; 
        while(bits > 0){
            //If there are more than 64 bits left generate 64 random bits then copy those into buffer
            if(bits >= high){
                uint64_t random_val; 
                _rdrand64_step(reinterpret_cast<unsigned long long*>(&random_val)); 
                std::copy_n(reinterpret_cast<uint8_t*>(&random_val), sizeof(random_val), std::back_inserter(buffer));
                bits -= 64; 
            }
            //if there are more than 32 bits left generate 32 random bits then copy those into buffer
            else if(bits < high && bits >= mid){
                uint32_t random_val; 
                _rdrand32_step(&random_val); 
                std::copy_n(reinterpret_cast<uint8_t*>(&random_val), sizeof(random_val), std::back_inserter(buffer)); 
                bits -= 32;
            }
            //if there are more than 16 bits left generate 16 random bits then copy those into buffer
            else if(bits < mid && bits >= low){
                uint16_t random_val;
                _rdrand16_step(&random_val); 
                std::copy_n(reinterpret_cast<uint8_t*>(&random_val), sizeof(random_val), std::back_inserter(buffer)); 
                bits -= 16;
            } 
            //if there are less than 16 bits generate 16 bits but use either the first 8 bits or all 16
            //depending on how many bits are left in the request
            else {
                uint16_t random_val;
                _rdrand16_step(&random_val); 
                std::copy_n(reinterpret_cast<uint8_t*>(&random_val), (bits > 8? 2:1), std::back_inserter(buffer)); 
                bits = 0;
                return true;
            }

        }
        if(!bits) return true; 
        return false;
   }

   //Generate random numbers with rdseed
   //Exact same process as rdrand
   bool rdseed(size_t bytes, buffer_t &buffer){
        int high = 64, mid = 32, low = 16, bits = static_cast<int>(bytes) * 8; 
        while(bits > 0){
            if(bits >= high){
                uint64_t random_val; 
                _rdseed64_step(reinterpret_cast<unsigned long long*>(&random_val)); 
                std::copy_n(reinterpret_cast<uint8_t*>(&random_val), sizeof(random_val), std::back_inserter(buffer));
                bits -= 64; 
            }
            else if(bits < high && bits >= mid){
                uint32_t random_val; 
                _rdseed32_step(&random_val); 
                std::copy_n(reinterpret_cast<uint8_t*>(&random_val), sizeof(random_val), std::back_inserter(buffer)); 
                bits -= 32;
            }
            else if(bits < mid && bits >= low){
                uint16_t random_val;
                _rdseed16_step(&random_val); 
                std::copy_n(reinterpret_cast<uint8_t*>(&random_val), sizeof(random_val), std::back_inserter(buffer)); 
                bits -= 16;
            } else {
                uint16_t random_val;
                _rdseed16_step(&random_val); 
                std::copy_n(reinterpret_cast<uint8_t*>(&random_val), (bits > 8? 2:1), std::back_inserter(buffer)); 
                bits = 0;
                return true;
            }

        }
        if(!bits) return true; 
        return false;
   }

    //Generate random numbers with random device
    bool randDev(size_t bytes, buffer_t &buffer){
        size_t startsize = buffer.size(); 
        std::random_device rd; 
        //pass in the requested bytes if less than 4 bytes are requested
        if(bytes < 4){
            auto temp = rd(); 
            std::copy_n(reinterpret_cast<uint8_t*>(&temp), bytes, std::back_inserter(buffer)); 
            return true; 
        }
        //keep generating random numbers until you reach the floor of bytes/4
        //push the correct number of bytes into buffer
        for(int i = 0; i < std::floor(bytes/4); i++){
            auto temp = rd(); 
            std::copy_n(reinterpret_cast<uint8_t*>(&temp), sizeof(temp), std::back_inserter(buffer)); 
        }
        //if there is a remainder for bytes / 4 this process will generate one more random number and insert the remainder into buffer
        if(bytes % 4){
            auto temp = rd(); 
            std::copy_n(reinterpret_cast<uint8_t*>(&temp), (bytes % 4), std::back_inserter(buffer)); 
        }

        if(buffer.size() - startsize) return true; 
        else return false; 
    }

    //Call this function if you do not want to specify the seed to start at
    bool LCG(size_t bytes, buffer_t &buffer){
        //seed will by randomly generated by rdseed
        uint32_t random_val; 
        _rdrand32_step(&random_val); 
        if(LCG(bytes, buffer, random_val)) return true; 
        return false; 
    }

    //Call this function if the seed is already specified it will fill buffer recursively 
    bool LCG(size_t bytes, buffer_t &buffer, uint32_t seed){
        //Calculate the number
        if(!bytes) return true; 
        uint64_t a = 6364136223846793005, c = 1, m = static_cast<uint64_t>(std::pow(2, 64)); 
        uint64_t x = (a*seed + c) % m; 
        uint8_t *ans = reinterpret_cast<uint8_t*>(&x);
        buffer_t seedarr; 
         
        //copy the middle 32 bits into buffer
        //copy the outer bits into the next seed
        copy_n(reinterpret_cast<uint8_t*>(&ans[2]), (bytes > 4? 4:bytes), std::back_inserter(buffer)); 
        copy_n(reinterpret_cast<uint8_t*>(&ans[0]), 2, std::back_inserter(seedarr));
        copy_n(reinterpret_cast<uint8_t*>(&ans[6]), 2, std::back_inserter(seedarr)); 
        
        //set bytes and seed for what they need to be for the next recursion
        bytes > 4? bytes -= 4:bytes = 0; 
        seed = seedarr[0] | (seedarr[1] << 8) | (seedarr[2] << 16) | (seedarr[3] << 24); 

        if(LCG(bytes, buffer, seed)) return true; 
        return false; 
    }

    
};
