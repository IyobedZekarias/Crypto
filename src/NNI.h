#include "limits.h"
#include <iomanip>
#include <stdexcept>
#include <tgmath.h>
#include <exception>
#include "crypto.h"


#ifndef _NNI_H__
#define _NNI_H__

class NNI {
private: 
    std::vector<uint64_t> nni; 
    void push(uint64_t num); 
    void DeleteZeros(); 
    bool testbit(size_t ind) const;
    
public:
    using digit_t = uint64_t;
    using b_digit_t = std::vector<digit_t>; 
    using l_digit_t = unsigned __int128;
    
    NNI(b_digit_t num); 
    NNI(crypto::buffer_t buf); 
    NNI(digit_t n);
    NNI(const char * str, int string); 
    NNI(); 
    
    //void divideby2();
    // NNI(const std::string &str);

    void randnni(size_t digits);
    size_t size() const;
    void clear(); 

    friend NNI divide(const NNI &u, const NNI &v, NNI &remainder);
    friend NNI modexp(const NNI &a, const NNI &e, const NNI &b);
    
    friend NNI operator+(const NNI &u,const NNI &v);
    friend NNI operator+=(const NNI &u,const NNI &v);
    friend void operator++(NNI &u);
    friend NNI operator-(const NNI &u,const NNI &v);
    friend void operator--(NNI &u);
    friend NNI operator-=(const NNI &u,const NNI &v);
    friend NNI operator*(const NNI &u,const NNI &v);
    friend NNI operator*=(const NNI &u,const NNI &v);
    friend NNI operator/(const NNI &u,const NNI &v);
    friend NNI operator/=(const NNI &u,const NNI &v);
    friend NNI operator%(const NNI &u,const NNI &v);
    friend NNI operator%=(const NNI &u,const NNI &v);
    friend NNI expmod(const NNI &a,const NNI &e,const NNI &b);
    NNI operator=(const digit_t &v);
    NNI operator=(const b_digit_t &v);

    friend bool operator<(const NNI &u,const NNI &v);
    friend bool operator<=(const NNI &u,const NNI &v);
    friend bool operator>(const NNI &u,const NNI &v);
    friend bool operator>=(const NNI &u,const NNI &v);
    friend bool operator==(const NNI &u,const NNI &v);
    friend bool operator==(const NNI &u,const NNI::digit_t dig);
    friend bool operator!=(const NNI &u,const NNI &v);

    friend NNI operator>>(const NNI &u, NNI::digit_t v);
    friend NNI operator<<(const NNI &u, NNI::digit_t v);
    friend NNI operator&(const NNI &u, const NNI &v);

    digit_t& operator[](const int i);
    friend std::ostream & operator<<(std::ostream &out,const NNI &priv);

};

#endif