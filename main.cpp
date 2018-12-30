#include <iostream>
#include <iomanip>
#include <bitset>
#include <cstdio>
#include <fstream>

#include "bool_func.h"

using namespace std;


int main( void )
{
#if 0
  uint N = 0b10011011;
  byte n = 3;

  uvec vec;
  vec.push_back(N);

  bool_func test(n, vec);

  uint x1 = 0b000, x2 = 0b001, x3 = 0b010, x4 = 0b110, x5 = 0b100;
  
  vec.clear();
  vec.push_back(x1);
  vec.push_back(x2);
  vec.push_back(x3);
  vec.push_back(x4);
  vec.push_back(x5);

  x1 = 0b011, x2 = 0b111, x3 = 0b101;

  vec.clear();
  vec.push_back(x1);
  vec.push_back(x2);
  vec.push_back(x3);

  //bool_func test(n, vec, tPCNF);

  /*
  for (int i = 0; i < 1 << n; i++)
    cout << bitset<3>(N = bool_func::Grey(i)) << "  " << dec << test.value(N) << endl;
  */
  for (int i = 0; i < 1 << n; i++)
    cout << bitset<3>(i) << "  " << dec << test.value(i) << endl;

  cout << test << endl;
  cin >> test;
  cout << test << endl;

#endif

  //uint //N = 0b0101100101000000;
         //N = 0b10011100;
         //N = 0b00000111111110001111100000000111;
         //N = 0x5540AABF;
  //uint N1= 0xAABF5540;
  uvec vec;
  //N = 0b01100100;
  /*
    00 1
    01 0
    11 1
    10 0

    1 0
    0 1
  */

  //N = 1001;
  //N = 0b11011110;
  //N = 993;
  //vec.push_back(N1);

#define NS 8
//#define N 0b11011110
//#define N 0b01100100
//#define N 0b10011100
//#define N 0b0101100101000000
//#define N 0b00000111111110001111100000000111
//#define N 0b10101000
//#define N 0x363636C9
//#define N1 0x66666699

  //vec.push_back(0x73D44D);
  //vec.push_back(0xB5ED49F2);
  //vec.push_back(0xFE988847);
  //vec.push_back(0x362F31E5);
  //vec.push_back(0x5FC21642);
  //vec.push_back(0x155FCE72);
  //vec.push_back(0x15AF2901);
  //vec.push_back(0);

  /// 0073D44D B5ED49F2 FE988847 362F31E5 5FC21642 155FCE72 15AF2901 00000000


  ifstream in;
  bool_func test;
  //bool_func test(NS, vec/*, tZheP*/);

  in.open("a.in");
  if (in.is_open())
    test.read(in);
  in.close();

  //for (int i = 0; i < 1 << NS; i++)
  //  cout << bitset<NS>(i) << "  " << dec << test.value(i, /*tZheP*/tRDNF/*tKarn*/) << endl;

  std::cout << endl << "save 0: " << test.saveZero() << endl;
  std::cout << "save 1: " << test.saveOne() << endl;
  std::cout << "self duality: " << test.isSelfDual() << endl;
  std::cout << "linear: " << test.isLinear() << endl;
  std::cout << "monotone: " << test.isMonotone() << endl << endl;

  ofstream out(stdout);
  test.write(out);
  test.write(out, tPDNF);
  test.write(out, tPCNF);
  test.write(out, tRDNF);
  test.write(out, tZheP);
  test.write(out, tKarn);

  getchar();

  return 0;
}
