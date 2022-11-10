#include "crypto_iz.h"
#include "fftw3.h"
using namespace crypto; 

class myexception: public std::exception
{
  virtual const char* what() const throw()
  {
    return "Subtracting small number by large number";
  }
} Subtraction;

namespace crypto{

void NNI::DeleteZeros(){
    for(auto i = nni.rbegin(); i != nni.rend(); i++){
        if(*i != 0 || nni.size() == 1) break; 
        nni.erase(( std::next(i).base() )); 
    }
}

inline int clz_u128 (NNI::l_digit_t u) {
  uint64_t hi = u>>64;
  uint64_t lo = u;
  int retval[3]={
    __builtin_clzll(hi),
    __builtin_clzll(lo)+64,
    128
  };
  int idx = !hi + ((!lo)&(!hi));
  return retval[idx];
}

NNI::NNI(digit_t n){
    nni.push_back(n);
}

NNI::NNI(const char * str, int string){
    NNI r; 
    NNI m(1); 
    for(size_t i = strlen(str)-1; i < strlen(str)+1; i--){
        r = r + (NNI(str[i] - '0') * m); 
        m = m * NNI(10); 
    }
    nni = r.nni; 
}

NNI::NNI(){
    nni.push_back(0); 
}   

NNI::NNI(b_digit_t num){
    nni.resize(num.size()); 
    std::memcpy(&nni[0], &num[0], sizeof(num[0]) * num.size()); 
}

NNI::NNI(crypto::buffer_t buf){
    const uint64_t* num_a = reinterpret_cast<const uint64_t*>(&buf[0]);
    double x = (double)buf.size()/8; 
    size_t n = std::ceil(static_cast<double>(buf.size())/8);
    b_digit_t num = std::vector<uint64_t>(num_a, num_a + n); 
    nni.resize(num.size()); 
    std::memcpy(&nni[0], &num[0], sizeof(num[0]) * num.size()); 
}

crypto::buffer_t NNI::toBuffer() const{
    crypto::buffer_t buf; 
    buf.resize(nni.size()*8); 
    std::memcpy(&buf[0], &nni[0], sizeof(nni[0]) * nni.size()); 
    return buf; 
}

size_t NNI::size() const{
    if(nni.size() == 1 && nni.at(0) == 0) return 0; 
    return nni.size(); 
}

void NNI::randnni(size_t digits){
    assert(digits > 0);
    crypto::buffer_t buffer;
    crypto::rdrand(digits*sizeof(NNI::digit_t),buffer);
    assert(buffer.size()>=digits);
    nni.resize(buffer.size()/8);
    std::memcpy(&nni[0], &buffer[0], sizeof(buffer[0]) * buffer.size()); 
    this->DeleteZeros(); 
    *this = *this / NNI(10); 
}

void NNI::clear(){
    nni.clear(); 
}

NNI::digit_t& NNI::operator[](const int i){
    return nni[i]; 
}

void NNI::push(digit_t num){
    nni.push_back(num); 
}

int print_uint128(NNI::l_digit_t n) {
  if (n == 0)  return printf("0\n");

  char str[40] = {0}; // log10(1 << 128) + '\0'
  char *s = str + sizeof(str) - 1; // start at the end
  while (n != 0) {
    if (s == str) return -1; // never happens

    *--s = "0123456789"[n % 10]; // save last digit
    n /= 10;                     // drop it
  }
  std::cout << std::string(s); 
  return 1;
}

//cout
std::ostream & operator<<(std::ostream &out, const NNI &priv){
    if(priv == 0) out << "0"; 
    NNI temp = priv; 
    char str[10000] = {0}; 
    char *s = str + sizeof(str) - 1;
    while (!(temp==0)) {  
        try{
            *--s = "0123456789"[(temp % NNI(10)).nni.at(0)]; 
        } catch (const std::out_of_range & e){
            *--s = "0"[0];
        }
        temp = temp / NNI(10); 
    } 
    out << std::string(s) << "\n"; 
    // for (size_t i=0; i<priv.size(); i++) {
    //     if (i%2 == 0)
    //         out << std::setw(4) << std::setfill(' ') << std::hex << i << ":";
    //     // std::bitset<64> y(priv.nni[i]); 
    //     // out << std::dec << y << ' '; 
    //     out << " " << std::setw(16) << std::setfill('0') << std::hex << priv.nni[i];
    //     if (i%2 == 1)
    //         out << "\n";
    // }
    // if (priv.size()%2)
    //     out << "\n";
    return out;
}

//bitshifts
NNI operator>>(const NNI &u, NNI::digit_t v){
    NNI a = u; 
    NNI::digit_t prev = 0; 
    if(v >= 64){
        for(int j = 0; j < v/64; j++){
            a.nni.erase(a.nni.begin()); 
        }
        v %= 64; 
    }
    for(auto i = a.nni.rbegin(); i != a.nni.rend(); i++){
        NNI::digit_t cur = *i << (64-v); 
        *i = (*i >> v) | prev; 
        if(*i == 0){
            a.nni.erase(std::next(i).base()); 
        }
        prev = cur; 
    }
    return a; 
}

NNI operator<<(const NNI &u, NNI::digit_t v){
    NNI a = u; 
    NNI::digit_t prev = 0; 
    if(v >= 64){
        for(int j = 0; j < v/64; j++){
            a.nni.insert(a.nni.begin(), 0); 
        }
        v %= 64; 
    }
    if(v){
        for(size_t i = 0; i < a.size(); i++){
            NNI::digit_t cur = a.nni[i] >> (64-v); 
            a.nni[i] = (a.nni[i] << v) | prev;
            if(i == a.size()-1){
                if(cur)
                    a.nni.push_back(0); 
            } 
            prev = cur; 
        }
    }
    return a; 
}

//Arithmetic
NNI operator+(const NNI &u,const NNI &v){
    NNI res;
    NNI::digit_t carry = 0; 
    auto i = u.nni.begin(); 
    //TODO: resize res instead of push_back
    for(auto j = v.nni.begin(); i != u.nni.end() | j != v.nni.end(); ){
        NNI::digit_t sum = 0; 
        NNI::digit_t ival = i == u.nni.end()? 0:*i, jval = j == v.nni.end()? 0:*j; 
        if(INT_ADD_OVERFLOW(jval, ival)){
            sum = jval + ival + carry; 
            carry = 1; 
        } else {
            sum = jval + ival; 
            if(INT_ADD_OVERFLOW(sum, carry)){
                sum += carry; 
                carry = 1; 
            } else {
                sum += carry; 
                carry = 0;  
            }
        }
        if(i != u.nni.end()) i++; 
        if(j != v.nni.end()) j++; 
        res.push(sum); 
    }

    if(carry)
        res.push(carry); 
    
    res.nni.erase(res.nni.begin()); 
    res.DeleteZeros(); 
    
    return res;
}

void operator++(NNI &u){
    u = u + NNI(1); 
}

NNI operator+=(const NNI &u,const NNI &v){
    return u + v; 
}

NNI & operator+=(NNI &u,const NNI &v){
    NNI temp = u; 
    u = temp + v;
    return u; 
}

NNI operator-(const NNI &u,const NNI &v){
    if(u < v) throw Subtraction; 
    NNI res;
    NNI::digit_t borrow = 0; 
    auto i = u.nni.begin();
    //TODO: resize res instead of push_back 
    for(auto j = v.nni.begin(); i != u.nni.end() | j != v.nni.end();){
        NNI::digit_t diff = 0; 
        NNI::digit_t ival = i == u.nni.end()? 0:*i, jval = j == v.nni.end()? 0:*j; 
        if(INT_SUB_OVERFLOW(ival, jval)){
            diff = ival - jval - borrow; 
            borrow = 1; 
        } else {
            diff = ival - jval; 
            if(INT_SUB_OVERFLOW(diff, borrow)){
                diff -= borrow; 
                borrow = 1; 
            } else {
                diff -= borrow; 
                borrow = 0; 
            }
        }
        if(i != u.nni.end()) i++; 
        if(j != v.nni.end()) j++; 
        res.push(diff); 
    }
    res.nni.erase(res.nni.begin()); 
    res.DeleteZeros(); 
    return res;
}

void operator--(NNI &u){
    u = u - NNI(1); 
}

NNI operator-=(const NNI &u,const NNI &v){
    return u - v; 
}

//Schönhage–Strassen
NNI operator*(const NNI &u,const NNI &v){
    // if(u == 0 || v == 0) return NNI(static_cast<NNI::digit_t>(0)); 
    // NNI a = u.size() > v.size() ? u:v; 
    // NNI b = u.size() > v.size() ? v:u; 


    NNI prod; 

    // size_t convleng = v.size() + u.size() - 1;
    // std::vector<NNI::l_digit_t> convolution(convleng, 0); 


    // NNI::l_digit_t convmat[b.size()][convleng] = {{0}}; 
    // for(size_t i = 0; i < b.size(); i++){
    //     memset(convmat[i], 0, sizeof(convmat[i])); 
    // }
    // //std::cout << a.size() << " " << b.size() << std::endl;
    // for(size_t i = 0; i < b.size(); i++){
    //     for(size_t j = i; j < i+a.size(); j++){
    //         convmat[i][j] = static_cast<NNI::l_digit_t>(b.nni.at(i)) * (a.nni.at(j-i)); 
    //         //std::cout << " " << b.nni.at(i) << " * " << a.nni.at(j-i); 
    //     }
    //     //std::cout << std::endl;
    // }


    // for(size_t i = 0; i < convleng; i++){
    //     for(int j = 0; j < b.size(); j++){
    //         convolution.at(i) += convmat[j][i]; 
    //     }
    // }   

    // NNI::digit_t* p; 
    // NNI prev = NNI(0);
    // uint64_t val; 
    // NNI::digit_t carry = 0; 
    // for(auto k = convolution.begin(); k!=convolution.end(); k++){
    //     NNI cur = prev + NNI(static_cast<uint64_t>(*k)) + NNI(carry);
    //     prev = NNI(static_cast<uint64_t>(*k>>64)); 
    //     carry = cur.size() > 1 ? cur.nni.at(1):0;
    //     val = cur.nni.at(0);  
    //     prod.nni.push_back(val);  
    // }
    // if(prev > NNI(0) || carry > 0){
    //     prod.nni.push_back(prev.nni.at(0) + carry); 
    // }
    // // for(auto k = convolution.begin(); k != convolution.end(); k++){
    // //     p = reinterpret_cast<NNI::digit_t *>(&*k); 
    // //     //std::cout << p[0] << ' ' << p[1] << std::endl;
    // //     //std::cout << static_cast<uint64_t>(*k) << std::endl;
    // //     val = 0;
    // //     if(INT_ADD_OVERFLOW(carry, prev)){
    // //         carry = 1; 
    // //     }
    // //     prev += carry; 
    // //     if(INT_ADD_OVERFLOW(prev, p[0])){
    // //         carry = 1; 
    // //     }
    // //     p[0] += prev; 
    // //     val += p[0]; 
    // //     //std::cout << val << ' ' << prev << std::endl;
    // //     prev = p[1]; 
    // //     prod.nni.push_back(val); 

    // // }
    
    // // val = 0; 
    // // if(INT_ADD_OVERFLOW(carry, prev)){
    // //     carry = 1; 
    // // } else {
    // //     carry = 0; 
    // // }
    // // val = carry + prev; 
    // // if(val)
    // //     prod.nni.push_back(val); 
    // // if(carry)
    // //     prod.nni.push_back(carry); 
    // prod.nni.erase(prod.nni.begin()); 
    //std::cout << "." << std::endl;
    prod.nni.resize(u.nni.size() + v.nni.size() + 1 , 0); 
    int j = 0;
    for(int i = 0; i < u.size(); i++){
        NNI::l_digit_t t = 0; 
        for(j = 0; j < v.size(); j++){
            t = t + static_cast<NNI::l_digit_t>(prod.nni.at(j+i)) + static_cast<NNI::l_digit_t>(u.nni.at(i)) * static_cast<NNI::l_digit_t>(v.nni.at(j)); 
            //NNI::digit_t * p = reinterpret_cast<NNI::digit_t*>(&t); 
            prod.nni.at(j+i) = static_cast<NNI::digit_t>(t); 
            t = t >> 64; 
        }
        prod.nni.at(j+i) = static_cast<NNI::digit_t>(t); 
    }
    
    prod.DeleteZeros(); 
    // std::cout << u << std::endl; 
    // std::cout << v << std::endl;; 
    // std::cout << prod << std::endl;; 
    return prod;
}

NNI operator*=(const NNI &u,const NNI &v){
    return u * v; 
}

NNI::l_digit_t qprime(NNI::digit_t u, NNI::digit_t u1, NNI::digit_t u2, NNI::digit_t v1, NNI::digit_t v2){
    NNI::l_digit_t qp = (static_cast<NNI::l_digit_t>(u << 1) + static_cast<NNI::l_digit_t>(u1)); 
    NNI::l_digit_t rp = qp / v1; 
    qp %= v1; 

    if((qp >> 64) > 0){
        qp -= 1; 
        rp += v1; 
    }
    while((rp >> 64) == 0 && qp * v2 > rp << 1 + u2){
        qp = qp -1; 
        rp = rp + v1; 
    } 
    return qp; 

}

//TODO: throw your own exception
NNI divide(const NNI &u, const NNI &v, NNI &remainder){
    if(v == 0) throw std::overflow_error("DIVIDE BY ZERO");  
    if(u == 0) return NNI(static_cast<NNI::digit_t>(0)); 
    if(u <= v){
        if(u == v) remainder.nni.clear(); 
        else remainder = u; 
        return NNI(static_cast<NNI::digit_t>(u==v));  
    }
    NNI quotient; 
    quotient.clear();

    NNI dividend = u;
    NNI divisor = v; 

    size_t m = dividend.size(), n = divisor.size(); 

    //case1
    if(m < n){
        remainder = dividend;
        return quotient;  
    }
    
    //case2
    else if(n < 2 && m < 2){
        quotient.nni.resize(1); 
        quotient.nni.at(0) = dividend.nni.at(0) / divisor.nni.at(0); 
        remainder.nni.at(0) = dividend.nni.at(0) % divisor.nni.at(0); 
        return quotient; 
    }
    //case 3
    else if(n < 2){
        NNI::digit_t div = 0;
        NNI::l_digit_t denom = 0; 
        NNI::b_digit_t doubledig;
        doubledig.resize(2, 0); 
        NNI::l_digit_t q = 0; 
        std::memcpy(&div, &divisor.nni[0], sizeof(divisor.nni[0]) * divisor.nni.size());
        quotient.nni.resize(m+1);   
        for(long int i = m-1; i >= 0; i--){        
            if(i == m-1){
                doubledig.at(0) = m-2 < m-1? dividend.nni.at(m-2) : 0; 
                doubledig.at(1) = dividend.nni.at(m-1); 
            } else {
                doubledig.at(0) = dividend.nni.at(i); 
                doubledig.at(1) = static_cast<NNI::digit_t>(denom); 
            }

            std::memcpy(&denom, &doubledig[0], sizeof(doubledig[0]) * doubledig.size());

            q = denom / div; 
            if(i == m-1){
                quotient.nni.at(i) = static_cast<NNI::digit_t>(q >> 64); 
                (i? quotient.nni.at(i-1) = static_cast<NNI::digit_t>(q):0); 
                i--; 
            } else {
                quotient.nni.at(i) = static_cast<NNI::digit_t>(q);
            }
            

            denom = denom - (q * div); 
 
        }

        remainder.nni.at(0) = static_cast<NNI::digit_t>(denom);
        quotient.DeleteZeros(); 
        return quotient;  
    }

    //case 4
    // int j = __builtin_clzll(divisor.nni.at(divisor.nni.size()-1)); 
    // divisor = divisor << j; 
    // remainder.clear();
    // remainder =  dividend << j; 
    // std::cout << "remainder: "<< remainder << std::endl;
    // remainder.nni.resize(dividend.size() + 1); 
    // for(long int k = m-n; k >= 0; --k){
    //     NNI::l_digit_t qp = qprime(remainder.nni.at(k+n), 
    //                                remainder.nni.at(k+n-1), 
    //                                remainder.nni.at(k+n-2), 
    //                                divisor.nni.at(n-1), 
    //                                divisor.nni.at(n-2));

    //     NNI::b_digit_t qpbuf = { static_cast<NNI::digit_t>(qp), static_cast<NNI::digit_t>(qp >> 64)}; 
    //     NNI w = divisor * (NNI(static_cast<NNI::digit_t>(qp)) << (k * 64));
    //     std::cout << remainder << std::endl;
    //     std::cout << w << std::endl;
    //     if(w > remainder){
    //         qp = qp - 1; 
    //         NNI::b_digit_t qpbuf2 = { static_cast<NNI::digit_t>(qp), static_cast<NNI::digit_t>(qp >> 64)}; 
    //         w = divisor * (NNI(static_cast<NNI::digit_t>(qp)) << (k * 64)); 
    //         std::cout << w << std::endl;
    //     }
    //     if(k > quotient.size()-1 || !quotient.size()) quotient.nni.resize(k+1);
    //     quotient.nni.at(k) = qp;

    //     std::cout << remainder << std::endl;
    //     std::cout << w << std::endl;
    //     remainder = remainder - w; 
    // }
    // remainder = remainder >> j; 
    // remainder.DeleteZeros();

    int shifts = 0;
    NNI::l_digit_t pow2_64 = 1;
    pow2_64 <<= 64; 

    shifts = __builtin_clzll(divisor.nni.at(n-1));
    divisor = divisor << shifts;  
    dividend.nni.push_back(0);
    dividend = dividend << shifts; 

    
    for(long int k = m-n; k >= 0; --k){
        NNI::l_digit_t rp = static_cast<NNI::l_digit_t>(dividend.nni.at(k+n)) * pow2_64 + static_cast<NNI::l_digit_t>(dividend.nni.at(k+n-1)); 
        NNI::l_digit_t qp = rp / static_cast<NNI::l_digit_t>(divisor.nni.at(n-1)); 
        rp %= divisor.nni.at(n-1); 


        // if(qp == pow2_64){
        //     qp -= 1; 
        //     rp += divisor.nni.at(n-1); 
        // }
        again:
        if(qp >= pow2_64 || 
            static_cast<NNI::digit_t>(qp) * static_cast<NNI::l_digit_t>(divisor.nni.at(n-2)) > pow2_64 * rp + dividend.nni.at(k+n-2)){
            qp -= 1; 
            rp += divisor.nni.at(n-1); 
            if(rp < pow2_64) goto again; 
        }
        
        // while(rp < pow2_64 && (qp * divisor.nni.at(n-2) > pow2_64 * rp + dividend.nni.at(k+n-2))){
        //     qp -= 1; 
        //     rp += divisor.nni.at(n-1); 
        // }
         

        // __int128_t carry = 0, widedigit = 0; 
        // __int128_t fs = 0xffffffffffffffffLL; 
        // for(long int i = 0; i < n; ++i){
        //     NNI::l_digit_t prod = static_cast<NNI::digit_t>(qp) * static_cast<NNI::l_digit_t>(divisor.nni.at(i)); 
        //     widedigit = (static_cast<NNI::l_digit_t>(dividend.nni.at(k+i)) + carry) - (prod & fs); 
        //     dividend.nni.at(k+i) = static_cast<NNI::digit_t>(widedigit); 
        //     carry = (widedigit >> 64) - (prod >> 64); 
        // }
        // widedigit = static_cast<NNI::l_digit_t>(dividend.nni.at(k+n)) + carry; 
        // dividend.nni.at(k+n) = widedigit; 
        __int128_t fs = 0xffffffffffffffffLL;
        __int128_t carry = 0, t; 
        for(int i = 0; i < n; i++){
            NNI::l_digit_t p = static_cast<NNI::digit_t>(qp) * static_cast<NNI::l_digit_t>(divisor.nni.at(i));  
            t = static_cast<NNI::l_digit_t>(dividend.nni.at(i+k) ) - carry - (p & fs); 
            dividend.nni.at(i+k) = static_cast<NNI::digit_t>(t); 
            carry = (p >> 64) - (t >> 64); 
        }   
        t = static_cast<NNI::l_digit_t>(dividend.nni.at(k+n)) - carry; 
        dividend.nni.at(k+n) = static_cast<NNI::digit_t>(t); 

        if(k > quotient.size()-1 || !quotient.size()) quotient.nni.resize(k+1);
        quotient.nni.at(k) = qp;

        // if(widedigit < 0){
        //     quotient.nni.at(k) -= 1; 
        //     widedigit = 0;
        //     for(long int i = 0; i < n; i++){
        //         widedigit += static_cast<NNI::l_digit_t>(dividend.nni.at(k+i)) + divisor.nni.at(i); 
        //         dividend.nni.at(k+i) = static_cast<NNI::digit_t>(widedigit); 
        //         widedigit >>= 64; 
        //     }
        //     dividend.nni.at(k+n) += static_cast<NNI::digit_t>(carry);  
        // }
        if(t < 0){
            quotient.nni.at(k) -= 1; 
            carry = 0; 
            for(int i = 0; i < n; i++){
                t = static_cast<NNI::l_digit_t>(dividend.nni.at(i+k)) + divisor.nni.at(i) + carry; 
                dividend.nni.at(i+k) = static_cast<NNI::digit_t>(t); 
                carry = t >> 64; 
            }
            dividend.nni.at(k+n) += static_cast<NNI::digit_t>(carry); 
        }
    }
    
    remainder = dividend >> shifts; 
    //remainder = u-(v*quotient); 
    remainder.DeleteZeros(); 
    quotient.DeleteZeros(); 
    // for(int i = 0; i < n-1; ++i){
    //     remainder.nni.at(i) = (dividend.nni.at(i) >> shifts) | 
    //         (static_cast<NNI::l_digit_t>(dividend.nni.at(i+1)) << (64-shifts)); 
    // }
    // remainder.nni.at(n-1) = dividend.nni.at(n-1) >> shifts; 
    // for(auto i = remainder.nni.rbegin(); i != remainder.nni.rend(); i++){
    //     if(*i != 0) break;
    //     remainder.nni.erase(std::next(i).base()); 
    // }
    // if(remainder != u - (v * quotient)){
    //     throw Subtraction; 
    // }

    return quotient; 
}


NNI operator/(const NNI &u,const NNI &v){
    NNI remainder; 
    return divide(u, v, remainder); 
}

NNI operator/=(const NNI &u,const NNI &v){
    return u / v; 
}

NNI operator%(const NNI &u,const NNI &v){
    NNI remainder; 
    divide(u, v, remainder); 
    return remainder; 
}

NNI operator%=(const NNI &u,const NNI &v){
    return u % v; 
}

NNI operator&(const NNI &u, const NNI &v){
    NNI a = u, b = v, res; 
    if(u.nni.size() < v.nni.size()) a.nni.resize(v.nni.size());
    if(v.nni.size() < u.nni.size()) b.nni.resize(u.nni.size());
    res.nni.resize(a.nni.size()); 
    for(int i = 0; i < a.nni.size(); i++){
        res.nni.at(i) = a.nni.at(i) & b.nni.at(i); 
    }
    return res; 
}

bool NNI::testbit(size_t ind) const{
    if(ind > nni.size()*64) return false; 
    return (nni.at(ind/64) >> (ind % 64)) & 0x1; 
    
}

NNI modexp(const NNI &a, const NNI &e, const NNI &b){

    NNI::l_digit_t lpow2 = 256;
    lpow2 *= lpow2; 
    lpow2 *= lpow2;  
    lpow2 *= lpow2; 
    NNI::digit_t pow2_64 = static_cast<NNI::digit_t>(lpow2 >> 1); 
    NNI r(1); 
    NNI p = a;
    p = p % b; 
    NNI::digit_t eval;
    for(auto i = e.nni.begin(); i != e.nni.end(); i++){
        eval = 0; 
        for(auto j = 1; j <= 64; j++){
            eval = *i; 
            eval <<= (64 - j);
            eval &= pow2_64; 
            
            if(eval >> 63){
                r = (r * p) % b; 
            }
            
            p = (p * p) % b; 
        } 

    }
    r.DeleteZeros(); 
    return r; 
}

//Conditional Statements
bool operator==(const NNI &u,const NNI &v){
    return u.nni == v.nni; 
}

bool operator==(const NNI &u,const NNI::digit_t dig){
    NNI::b_digit_t v = {dig};  
    return u.nni == v; 
}

bool operator!=(const NNI &u,const NNI &v){
    return u.nni != v.nni; 
}

bool operator<(const NNI &u,const NNI &v){
    if(u.size() < v.size()) return 1;
    else if (u.size() > v.size()) return 0;  

    auto ui = u.nni.rbegin(); 
    for(auto vi = v.nni.rbegin(); vi != v.nni.rend(); vi++, ui++){
        if(*ui < *vi) return 1; 
        else if (*ui > *vi) return 0; 
    }
    return 0; 
}

bool operator>(const NNI &u,const NNI &v){
    return (v < u); 
}

bool operator<=(const NNI &u,const NNI &v){
    return !(u > v); 
}   

bool operator>=(const NNI &u,const NNI &v){
    return !(u < v); 
}

NNI NNI::operator=(const digit_t &v){
    return NNI(v); 
}

NNI NNI::operator=(const b_digit_t &v){
    return NNI(v); 
}
}