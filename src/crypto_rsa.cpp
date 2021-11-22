#include "crypto.h"
#include "NNI.h"


#define RSAbits 4096

// NNI * list = {
//    NNI(""),
// }; 

// std::ostream & operator<<(std::ostream &out,const crypto::RSApublic64 &pub){
//    const uint64_t* n = reinterpret_cast<const uint64_t*>(&pub.n);
//    out << "n: " <<  n[0] << ' ' << n[1] << "\n"; 
//    out << "e: " << pub.e << "\n"; 
//    return out; 
// }

// std::ostream & operator<<(std::ostream &out,const crypto::RSAprivate64 &priv){
//    const uint64_t* n = reinterpret_cast<const uint64_t*>(&priv.n);
//    out << "n: " <<  n[0] << ' ' << n[1] << "\n"; 
//    out << "p: " << priv.p << "\n"; 
//    out << "q: " << priv.q << "\n"; 
//    const uint64_t* d = reinterpret_cast<const uint64_t*>(&priv.d);
//    out << "d: " <<  d[0] << ' ' << d[1] << "\n"; 
//    const uint64_t* phi = reinterpret_cast<const uint64_t*>(&priv.phi);
//    out << "phi: " <<  phi[0] << ' ' << phi[1] << "\n"; 
//    //out << "phi: " <<  priv.phi << "\n";
//    return out; 
// }

#define ll crypto::uint128_t
using namespace std;
NNI mulmod(NNI a, NNI b, NNI m){//It returns true if number is prime otherwise false {
   NNI x ,y = a % m;
   while (b.size() > 0) {
      if (b % NNI(2) == 1) {
         x = (x + y) % m;
      }
      y = (y * 2) % m;
      b = b / 2;
   }
   return x % m;
}

ll modulo(ll base, ll e, ll m) {
   ll x = 1;
   ll y = base;
   while (e > 0) {
      if (e % 2 == 1)
         x = (x * y) % m;
         y = (y * y) % m;
         e = e / 2;
   }
   return x % m;
}

bool Miller(NNI p, int iteration) {
   if (p < NNI(2)) {
      return false;
   }
   if (p != NNI(2) && p % NNI(2) == 0) {
      return false;
   }
   NNI s = p - NNI(1);
   while (s % NNI(2) == 0) {
      s = s >> 1;
   }
   for (int i = 0; i < iteration; i++) {
      NNI rnni; 
      
      // while(true){
      //    rnni.randnni(p.size());
      //    if(rnni < (p - NNI(1))) break; 
      //    std::random_device rd;
      //    std::uniform_int_distribution<int> roll(0,(p.size()*64));
      //    while(rnni >= (p-NNI(1))){
      //       int i = roll(rd);
      //       rnni = rnni >> (i); 
      //    }
      //    if(rnni < (p - NNI(1)) && rnni > NNI(1)) break; 
      // }
      // std::cout << rnni << std::endl;
      // std::cout << p << std::endl;
      
      // if(mod == 1 || mod == (p - NNI(1))) {i++; goto loop;} 
      // for(int r = 0; r < sr-1; r++){
      //    mod = modexp(mod, NNI(2), p); 
      //    if(mod == 1) return false; 
      //    if(mod == (p - NNI(1))) {i++; goto loop;} 
      // }
      // return false; 



      rnni.randnni(p.size()); 
      rnni = rnni % (p - NNI(1)); 
      NNI d = s;
      NNI mod = modexp(rnni, d, p);
      while (d != p - NNI(1) && mod != NNI(1) && mod != p - NNI(1)) {
         mod = modexp(mod, NNI(2), p);
         d = d * NNI(2);
      }
      if (mod != p - NNI(1) && d % NNI(2) == 0) {
         return false;
      }
   }
   return true;
}

typedef struct tag_uint128AndSign {
    crypto::uint128_t  value;
    bool isNegative;
} uint128AndSign;


crypto::uint128_t mul_inv(crypto::uint128_t a, crypto::uint128_t b)
{
    if (b <= 1)
        return 0;

    crypto::uint128_t b0 = b;
    uint128AndSign x0 = { 0, false }; // b = 1*b + 0*a
    uint128AndSign x1 = { 1, false }; // a = 0*b + 1*a

    while (a > 1)
    {
        if (b == 0) // means original A and B were not co-prime so there is no answer
            return 0;
        crypto::uint128_t q = a / b;
        // (b, a) := (a % b, b)
        // which is the same as
        // (b, a) := (a - q * b, b)
        crypto::uint128_t t = b; b = a % b; a = t;

        // (x0, x1) := (x1 - q * x0, x0)
        uint128AndSign t2 = x0;
        crypto::uint128_t qx0 = q * x0.value;
        if (x0.isNegative != x1.isNegative)
        {
            x0.value = x1.value + qx0;
            x0.isNegative = x1.isNegative;
        }
        else
        {
            x0.value = (x1.value > qx0) ? x1.value - qx0 : qx0 - x1.value;
            x0.isNegative = (x1.value > qx0) ? x1.isNegative : !x0.isNegative;
        }
        x1 = t2;
    }

    return x1.isNegative ? (b0 - x1.value) : x1.value;
}

void gennumber(uint64_t &num){
   crypto::buffer_t k;
   const uint64_t* p; 
   while(true){
      crypto::rdrand((RSAbits/32)+2, k); 
      p = reinterpret_cast<const uint64_t*>(&k[0]);
      if(p[0] > 0x8000) break; 
   }
   num = p[0]; 
    
}

crypto::uint128_t modular_pow(crypto::uint128_t b, crypto::uint128_t exp, crypto::uint128_t m){
   if(m == 1) return 0; 
   crypto::uint128_t c = 1; 
   b = b % m; 
   if(b == 0) return 0; 
   while(exp > 0){
      if(exp & 1)
         c = (c * b) % m; 
      exp = exp >> 1; 
      b = (b * b) % m;
   }
   return c; 
}

namespace crypto {
    bool generate_rsa64(RSAprivate64 &key, RSApublic64 &pub){
      NNI p, q;
      p.randnni(32);
      q.randnni(32);
      if (p % NNI(2) == 0) p = p + NNI(1); 
      if (q % NNI(2) == 0) q = q + NNI(1); 
      for(;;){
         // std::vector<uint64_t> rand = { 0xDA29C99B881A9A85, 0xC73CAB7C0BD64579, 0x3B4C25323B };
         // buffer_t rand; 
         // crypto::rdrand(8*4, rand); 
         // rand.at(0) |= 1; 
          
         //std::cout << p << std::endl;
         if(Miller(p, 40)){
            // file.open("primes", std::ios::out| std::ios::binary | std::ios_base::app); 
            std::cout << std::endl << p; 
            break;
            // file << p;
            // file.close(); 
            
         } else {
            std::cout << "." << std::flush;
            p = p + NNI(2); 
            
         }
      } 

      for(;;){
         // std::vector<uint64_t> rand = { 0xDA29C99B881A9A85, 0xC73CAB7C0BD64579, 0x3B4C25323B };
         // buffer_t rand; 
         // crypto::rdrand(8*4, rand); 
         // rand.at(0) |= 1; 
          
         //std::cout << p << std::endl;
         if(Miller(q, 40)){
            // file.open("primes", std::ios::out| std::ios::binary | std::ios_base::app); 
            std::cout << std::endl << q; 
            break;
            // file << p;
            // file.close(); 
            
         } else {
            std::cout << "." << std::flush;
            q = q + NNI(2); 
            
         }
      }
      
      // NNI p(dig4); 
      // std::cout << p << std::endl;
      // std::cout << Miller(p, 40) << std::endl;
      // uint32_t e; 
      // if(pub.e){
      //    e = pub.e;
      //    goto genD;     
      // }
      // else e = 65537;   

      // restart: 
      // gennumber(key.p); 
      // if (!(key.p & 1))
      //       key.p += 1;
      // while(true){
      //    if(Miller(key.p, 40) && (key.p % e) != 1) break; 
      //    else key.p += 2; 
      // }

      // gennumber(key.q); 
      // if (!(key.q & 1))
      //       key.q += 1;
      // while(true){
      //    if(Miller(key.q, 40) && (key.q % e) != 1) break; 
      //    key.q += 2;
      // }

      // genD:
      // uint128_t n = key.p * key.q; 
      // uint128_t phi = (key.p-1) * (key.q-1); 
      // key.d = mul_inv(e, phi); 
      // pub.n = n; 
      // pub.e = e;
      // key.n = n;
      // key.phi = phi;  
      // std::cout << "private: \n" << key; 
      // std::cout << "public: \n" << pub; 
      // std::cout << key.p << "\n" << key.q << "\n" << d[0] << "\n" << pub.e << std::endl;
      // const uint64_t* p = reinterpret_cast<const uint64_t*>(&phi);
     

      return true; 
    }


      bool encode_rsa64(const buffer_t message, buffer_t &cipher, const RSApublic64 &pub){
         if(message.size() > 8) return false; 
         //const uint128_t* m = reinterpret_cast<const uint128_t*>(&message[0]); 
         uint128_t m = 0;
         std::memcpy(&m, &message[0], sizeof(message[0]) * message.size()); 
         if(m >= pub.n) return false; 

         uint128_t c = modular_pow(m, pub.e, pub.n); 
         // const uint64_t* p = reinterpret_cast<const uint64_t*>(&c);
         // const uint64_t* q = reinterpret_cast<const uint64_t*>(&m[0]);
         // std::cout << std::dec << q[0] << std::endl;
         // std::cout << p[0] << ' ' << p[1] << std::endl;
         const uint8_t* cip = reinterpret_cast<const uint8_t*>(&c); 
         cipher = std::vector<uint8_t>(cip, cip + sizeof(cip)/sizeof(cip[0])); 
         cipher.resize(message.size()); 
         return true; 
      }


       bool decode_rsa64(buffer_t &message, const buffer_t cipher, const RSAprivate64 &key){
         uint128_t c = 0; 
         std::memcpy(&c, &cipher[0], sizeof(cipher[0]) * cipher.size()); 
         //const uint128_t* c = reinterpret_cast<const uint128_t*>(&cipher[0]);
         
         uint128_t m = modular_pow(c, key.d, key.n); 
         const uint8_t* mes = reinterpret_cast<const uint8_t*>(&m); 
         message = std::vector<uint8_t>(mes, mes + sizeof(mes)/sizeof(mes[0])); 
         message.resize(cipher.size()); 
         return true; 
      }
};