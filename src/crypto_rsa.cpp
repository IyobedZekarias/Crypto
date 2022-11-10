#include "crypto_iz.h"

using namespace std;
using namespace crypto; 

std::ostream & operator<<(std::ostream &out,const crypto::RSApublic &pub){
   out << "n: " << *pub.n << "\n"; 
   out << "e: " << *pub.e << "\n"; 
   return out; 
}

std::ostream & operator<<(std::ostream &out,const crypto::RSAprivate &priv){
   out << "n: " << *priv.n << "\n"; 
   out << "p: " << *priv.p << "\n"; 
   out << "q: " << *priv.q << "\n"; 
   out << "d: " << *priv.d << "\n"; 
   out << "phi: " << *priv.phi << "\n"; 
   //out << "phi: " <<  priv.phi << "\n";
   return out; 
}



typedef struct NNIandSign {
   NNI value;
   bool isNegative;
} NNIandSign;


NNI mul_inv(NNI a, NNI b){
   if (b <= NNI(1))
      return NNI("0");

   NNI b0 = b;
   NNIandSign x0 = { NNI("0"), false }; // b = 1*b + 0*a
   NNIandSign x1 = { NNI(1), false }; // a = 0*b + 1*a

   while (a > NNI(1))
   {
      if (b == 0) // means original A and B were not co-prime so there is no answer
         return NNI("0");
      NNI q = a / b;
      // (b, a) := (a % b, b)
      // which is the same as
      // (b, a) := (a - q * b, b)
      NNI t = b; b = a % b; a = t;

      // (x0, x1) := (x1 - q * x0, x0)
      NNIandSign t2 = x0;
      NNI qx0 = q * x0.value;
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


namespace crypto {
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

   bool generate_rsa(RSAprivate *key, RSApublic *pub){
      NNI p, q;
      *pub->e = NNI(65537);

      p.randnni(32); q.randnni(32);
      if (p % NNI(2) == 0) p = p + NNI(1); 
      std::cout << "generating primes will take some time" << std::endl;
      int counter = 0; 
      for(;;){
         if(Miller(p, 40) && p % *pub->e != NNI(1)){
            break;
            
         } else {
            if(counter % 10 == 0) std::cout << "." << std::flush;
            p = p + NNI(2); 
            counter++; 
            
         }
      } 

      if (q % NNI(2) == 0) q = q + NNI(1); 
      counter = 0; 
      while(true){
         if(Miller(q, 40) && q % *pub->e != NNI(1)){
            break; 
         } else {
            if(counter % 10 == 0) std::cout << "." << std::flush;
            q = q + NNI(2); 
            counter++; 
            
         }
      }
      std::cout << std::endl;

      *key->n = p * q; 
      *key->phi = (p - NNI(1)) * (q - NNI(1)); 
      *key->d = mul_inv(*pub->e, *key->phi); 
      *key->p = p; 
      *key->q = q; 
      *pub->n = *key->n; 
   
      return true; 
   }


   bool encode_rsa(const buffer_t message, buffer_t &cipher, const RSApublic &pub){
      NNI m = NNI(message); 
      if(!(NNI(1) < m < *pub.n)) return false; 
      cipher = modexp(m, *pub.e, *pub.n).toBuffer(); 
      // std::cout << m << std::endl;
      // std::cout << *pub.e << std::endl;
      // std::cout << *pub.n << std::endl;
      return true; 
   }


   bool decode_rsa(buffer_t &message, const buffer_t cipher, const RSAprivate &key){
      NNI c(cipher); 
      message = modexp(c, *key.d, *key.n).toBuffer(); 
      // std::cout << c << std::endl; 
      // std::cout << *key.d << std::endl; 
      // std::cout << *key.n << std::endl;
      return true; 
   }
};