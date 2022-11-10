#include "../src/crypto_iz.h"
// #include <iostream>
// #include <cassert>
// #include <random>
// #include "../src/crypto_iz.h"
using namespace std;
using namespace crypto; 
void set_n_rand_digits(NNI &n,size_t digits)
{
    assert(digits > 0);
    crypto::buffer_t buffer;
    crypto::rdrand(digits*sizeof(NNI::digit_t),buffer);
    assert(buffer.size()>=digits);
    n = NNI(buffer);
    assert(!(n==0));
}
std::random_device rd;
void test(int min_digits,int max_digits)
{
    NNI a,b,c,d,e,f;
    a.randnni(20); 
    b.randnni(20); 
    c.randnni(20); 
    d.randnni(20); 
    // std::uniform_int_distribution<int> roll(min_digits,max_digits);
    // set_n_rand_digits(a,roll(rd));
    // set_n_rand_digits(b,roll(rd));
    // set_n_rand_digits(c,roll(rd));
    // set_n_rand_digits(d,roll(rd));
    // test add & subtract
    f = a+b+c+d;
    e = f-a-b-c-d;
    if (!(e == 0)) {
        cout << "e=" << e; 
        cout << "a=" << a;
        cout << " b=" << b;
        cout << " c=" << c;
        cout << " d=" << d;
        cout << " a+b+c+d-a-b-c-d=" << f <<endl<<endl;        
    }
    
    NNI::digit_t x = std::uniform_int_distribution<int>(1,1000)(rd);
    NNI r{x};
    f = a*b*c*d+r;
    //cout << f << std::endl;
    e = divide(f,a,r);
    if (!(r == x)) {
        cout << "a= " << a;
        cout << " b= " << b;
        cout << " c= " << c;
        cout << " d= " << d;
        cout << " r= " << r;
        cout << " x= " << x << endl;
        cout << "a*b*c*d+x=" << f <<endl;
        cout << "(a*b*c*d+x)/a=" << e <<endl;
        cout << "a*b*c*d+x mod a=" << r <<endl<<endl;
    }
    f = e+r;
    e = divide(f,b,r);
    if (!(r == x)) {
        cout << "a= " << a;
        cout << " b= " << b;
        cout << " c= " << c;
        cout << " d= " << d;
        cout << " r= " << r;
        cout << " x= " << x << endl;
        cout << " (a*b*c*d+x)/a mod b=" << r <<endl<<endl;
    }     
    f = e+r;
    e = divide(f,c,r);
    if (!(r == x)) {
        cout << "a= " << a;
        cout << " b= " << b;
        cout << " c= " << c;
        cout << " d= " << d;
        cout << " r= " << r;
        cout << " x= " << x << endl;
        cout << " ((a*b*c*d+x)/a+x)/b mod c= " << r <<endl<<endl;
    }     
    f = e+r;
    e = divide(f,d,r);
    if (!(r == x)) {
        cout << "a= " << a;
        cout << " b= " << b;
        cout << " c= " << c;
        cout << " d= " << d;
        cout << " r= " << r;
        cout << " x= " << x << endl;
        cout << " (((a*b*c*d+x)/a+x)/b+x/c) mod d=" << r <<endl<<endl;
    }     
    if (!(e == 1)) {
        cout << "e= " << e; 
        cout << "a= " << a;
        cout << " b= " << b;
        cout << " c= " << c;
        cout << " d= " << d;
        cout << " x= " << x << endl;
        cout << " ((((a*b*c*d+x)/a+x)/b+x)/c+x)/d=" << e <<endl<<endl;
    }     
}
int main(int argc,char *argv[])
{   
    if (argc < 2) {
        cout << "usage: " << argv[0] << " <min-digits> [<max-digits>]\n";
        exit(1);
    }
 
    int min = stoi(argv[1]);
    int max = argc>2 ? stoi(argv[2]) : min;
    cout << "testing " << (min*sizeof(NNI::digit_t)*8-7) << ".." << 
    max*sizeof(NNI::digit_t)*8 << " bits\n";
    while (true) {
        test(min,max);
        cout << "." << flush;
    }
    buffer_t buf; 
    rdrand(7, buf); 
    cout << buf; 
    NNI a(buf); 
    cout << a; 
}


int print_uint128(uint128_t n) {
  if (n == 0)  return printf("0\n");

  char str[40] = {0}; // log10(1 << 128) + '\0'
  char *s = str + sizeof(str) - 1; // start at the end
  while (n != 0) {
    if (s == str) return -1; // never happens

    *--s = "0123456789"[n % 10]; // save last digit
    n /= 10;                     // drop it
  }
  return printf("%s\n", s);
}

// int main(){
//     { 
//       cout << "ADDITION" << endl;
//       uint64_t dig1 = 0xffffffffffffffff;
//       uint64_t dig2 = 0xffffffffffffffff;
//       std::vector<uint64_t> dig3 = {0x0,  0x9 };
//       std::vector<uint64_t> dig4 = {0x2};
//       //uint64_t sum = dig1 + dig2; 
//       NNI a(dig3); 
//       NNI b(dig4);
//       cout << a; 
//       cout << b; 

//       NNI c;
//       c = a - b;   
//       //cout << hex << sum << endl;
//       cout << c << endl; 
//       cout << b % NNI(10); 
//       //cout << hex << setw(16) << setfill('0') << c[0] << ' ' << hex << setw(16) << setfill('0') << c[1] << endl;
//     }
//     {
//       cout << "MULTIPLICATION" << endl;
//       // uint64_t dig1 = 2;
//       // uint64_t dig2 = 4;
//       // uint128_t a = static_cast<uint128_t>(dig1) * static_cast<uint128_t>(dig2);
//       // const uint64_t* n = reinterpret_cast<const uint64_t*>(&a);
//       // //cout << hex << setw(16) << setfill('0') << n[0] << ' ' << setw(16) << setfill('0') << n[1] << endl;  
//       // //print_uint128(a); 

//     //   std::vector<uint64_t> dig3 = {0x4A53F4DD431109C8,  0x6BD3EA4E65A };
//     //   std::vector<uint64_t> dig4 = {0x094343DDCC2F5EB1,  0xAA3257D1AEEC0BC, 0XAF02384BF89FFF45, 0XAF0238b5F67B432E};
//     //   std::vector<uint64_t> dig3 = { 0x8000000000000000, 0x0000000000000001 };
//     //   std::vector<uint64_t> dig4 = { 0x8000000000000000, 0x0000000000000003 };
//       // // std::vector<uint64_t> dig3 = { 0x1, 0x2, 0x3, 0x4, 0x5, 0x6 };
//       // // std::vector<uint64_t> dig4 = { 0x1, 0x2 };
//       //buffer_t buf1, buf2, buf3, buf4; 
//       // rdrand(56, buf1); 
//       // rdrand(42, buf2); 
//       // rdrand(36, buf3); 
//       // rdrand(56, buf4);
//       NNI y, z, w, x;
//       set_n_rand_digits(y, 5); 
//       set_n_rand_digits(z, 7); 
//       set_n_rand_digits(w, 1); 
//       set_n_rand_digits(x, 6); 
//       // NNI y(buf1); 
//       // NNI z(buf2); 
//       // NNI w(buf3); 
//       // NNI x; 
//       // for(;;){
//       //   buf1.clear(); 
//       //   buf2.clear(); 
//       //   rdrand(8, buf1);
//       //   rdrand(8, buf2); 
//       //   uint64_t test1, test2;
//       //   std::memcpy(&test1, &buf1[0], sizeof(buf1[0]) * buf1.size()); 
//       //   std::memcpy(&test2, &buf2[0], sizeof(buf2[0]) * buf2.size()); 
//       //   uint128_t ans = static_cast<uint128_t>(test1) * test2; 
//       //   uint64_t *p = reinterpret_cast<uint64_t*>(&ans); 
//       //   x = NNI(buf1) * NNI(buf2); 
//       //   // std::cout << "buf1: " << NNI(buf1) << std::endl; 
//       //   // std::cout << "buf2: " << NNI(buf2) << std::endl; 
//       //   // std::cout << "x: " << p[0] << std::endl; 

//       //   if(x[0] == p[0] && x[1] == p[1]){
//       //     std::cout << "."; 
//       //   } else{
//       //     std::cout << "1: " << test1 << std::endl;
//       //     std::cout << "2: " << test2 << std::endl;
//       //     std::cout << "answer: "; print_uint128(ans); 
//       //     std::cout << x << std::endl; 
//       //     break;
//       //   }
//       // }
//       cout << y << endl;
//       cout << z << endl;
//       // cout << w << endl;
//       // cout << x << endl;
//       NNI c = y * z; 
//       cout << c << endl; 
//       while(c.size() > 1)
//         c = c / NNI(10); 
//       cout << c << endl; 

//     //cout << c.size() << hex << setw(16) << setfill('0') << c[0] << ' ' << hex << setw(16) << setfill('0') << c[1] << endl;
//     }
//     {
//         NNI a(1),b(7),c(256);
//         c = c * c;  // 65536
//         c = c * c;  // 4B
//         c = c * c;  // 1,0
//         a = a * c;  // 1,0
//         b = b * c;  // 7,0
//         c = a + b;  // 8,0
//         assert(c.size()==2);
//         assert(c[0]==0);
//         assert(c[1]==8);
//     }
//     {
//         NNI a(10000),b(30000),c(256),d(1234);
//         c = c * c;
//         c = c * c;
//         c = c * c;
//         a = a * c + d;      // 10000,1234
//         b = b * c + d;      // 30000,1234
//         d = a + b;          // 40000,2468
//         assert(d.size()==2);
//         assert(d[0]==1234*2);
//         assert(d[1]==40000);
//     }
//     {
//         NNI c(256); 
//         c = c*c; 
//         c = c*c;
//         c = c*c;
//         NNI a(4);
//         a = a * c + NNI(3);     // 4,3
//         a = a * c + NNI(2);     // 4,3,2
//         NNI b(7);               //
//         b = b * c + NNI(6);     // 7,6
//         b = b * c + NNI(5);     // 7,6,5
//         //          4   3   2
//         //       x  7   6   5
//         //       ------------
//         //         20  15  10
//         //     24  18  12
//         // 28  21  14
//         c = a * b;
//         assert(c.size()==5);
//         assert(c[0] == 2*5);
//         assert(c[1] == 3*5+2*6);
//         assert(c[2] == 4*5+6*3+7*2);
//         assert(c[3] == 4*6+7*3);
//         assert(c[4] == 4*7);
//     }
//     // {
//     //   cout << "DIVISION BY 2" << endl;
//     //   NNI::b_digit_t dig = { 0xB6F3CDBDFC293C2A, 0x35D8684E1D2835A0, 0x05B2412136A2 }; 
//     //   NNI c(dig); 
//     //   cout << c << endl;
//     //   c.divideby2(); 
//     //   cout << c; 
//     // }
//     {
//       cout << "DIVISION" << endl;
//       for(int i = 0; i < 10; i++){
//         NNI a, b, q, r; 
//         a.randnni(10); 
//         b.randnni(9);
//         q = divide(a, b, r); 
//         std::cout << "a:\n" << a << endl; 
//         std::cout << "b:\n" << b << endl; 
//         std::cout << "a/b:\n" << q << endl; 
//         std::cout << "a mod b:\n" << r << endl; 
//       }
//       // std::vector<uint64_t> dig3 = {0x0000000000000000, 0x0000000000000024};
//       // std::vector<uint64_t> dig4 = {0x0000000000000002, 0x0000000000000008};
//       // NNI a(dig3); 
//       // NNI b(dig4); 
//       // cout << a; 
//       // cout << b << endl;
//       // NNI c = a/b; 
//       // cout << c << endl; 
//     }
//     {
//       cout << "BITSHIFTS" << endl;
//       std::vector<uint64_t> dig3 = {0b100010101010111011,  0b010111010010101 };
//       //std::vector<uint64_t> dig4 = {0x094343DDCC2F5EB1,  0x3257D13DA4F};
//       NNI c(dig3); 
//       cout << c << endl;
//       c = c << (64*2); 
//       cout << c << endl;
//       c = c >> 192; 
//       cout << c;  
//     }
//     {
//       cout << "EXPONENTIAL MOD" << endl;
//       buffer_t abuf, ebuf, bbuf; 
//       for(int i = 0; i < 10; i++){
//         // NNI a("710678029416949805183905811075996848348828794610158559814400062180900562076305940297325188368435979698694290217335140632737155972867988482004005274381902562259022622706753540874971105611251018631818845876887583088044656543002749425", 1);
//         // NNI e("28947450446169745536792987916997361550962107444872220349063573334708709940753795318649379581415896183590843735381271", 1);
//         // NNI b{"374425485574336491055655550119847958070282886057845109270468345231861842888909097754890711017125955490931144125065561319963615272988301842336379710144490643694856454331849443148338863612157114527480233358092801853381392490249125639096062690611164358707646988648452041572765572885171740810367948442906027360032771546552377391908143175389022443029780083741203365685158588502939181437848312977948968434065860", 1}; 
//         NNI a, b, e;
//         set_n_rand_digits(a, 3); 
//         set_n_rand_digits(e, 3); 
//         set_n_rand_digits(b, 3); 
//         cout << "a\n" << a << endl; 
//         cout << "e\n" << e << endl; 
//         cout << "b\n" << b << endl;
//         NNI ans = modexp(a, e, b); 
//         std:cout << ans << std::endl; 

//       }
      
//       //cout << (NNI(1) * a) % b << endl; 
      
//     }
//     {
//       cout << "TESTS" << endl;
//       NNI a = 5; 
//       cout << a; 
//     }
//     return 0; 
// }