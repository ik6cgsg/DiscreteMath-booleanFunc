#ifndef _BOOL_FUNC_H__
#define _BOOL_FUNC_H__
#pragma once

#include <vector>
#include <iostream>

using uint = unsigned int;         // 32 bits
using luint = long unsigned int;   // 64 bits
using uvec = std::vector<uint>;    // package of 32bits numbers
using umatr = std::vector<uvec>;   // matrix of packages of 32bits numbers for tKarn
using byte = unsigned char;

// Forward declaration
class bool_func;

// I/O methods for truth table
std::istream & operator>>(std::istream & in, bool_func & bf);
std::ostream & operator<<(std::ostream & out, bool_func & bf);

enum rep_type
{
  tTVT, tPDNF, tPCNF, tZheP, tKarn, tRDNF, tCF, tDF
}; // end of 'rep_type' enum

// Class definition
class bool_func
{
private:
  friend std::istream & operator>>(std::istream & in, bool_func & bf);
  friend std::ostream & operator<<(std::ostream & out, bool_func & bf);

  byte n;
  uint mPDNF, mPCNF, mRDNF, mCF, mDF;
  uvec TVT, PDNF, PCNF, ZheP, RDNF, CF, DF;
  umatr Karn;

  // auxiliary functions
  void convers(rep_type typeFrom, std::vector<rep_type> typesTo, bool needQuine);
  void pack(uvec inv, rep_type tp, uvec ent = uvec());   // RDNF, PDNF, PCNF
  uint getSet(uint i, uvec bf, luint m);  // RDNF, PDNF, PCNF, CF, DF
  void print(std::ostream & out, int w, uvec bf);
  bool consistPair(uvec & entV, uvec & invV, uint ent, uint inv);
  static uint Grey(uint ind);      // From index to Grey code
  static uint Grey2Int(uint g);    // From Grey code to index

  // For TVT
  void setValue(uint x, bool val);

  // Convertors
  void fromTVT(rep_type tp);
  void toTVT(rep_type tp = tTVT);
  void Quine();  // PDNF -> RDNF
public:
  bool_func() = default;
  bool_func(byte n, const uvec & source, rep_type tp = tTVT, luint m = 0);
  bool_func(byte n, const umatr & KarnMatr);

  // I/O
  void read(std::istream & in, rep_type tp = tTVT, luint m = 0);
  void write(std::ostream & out, rep_type tp = tTVT);

  // Getting value
  bool value(uint x, rep_type tp = tTVT);

  // Check properties
  bool saveZero();
  bool saveOne();
  bool isSelfDual();
  bool isLinear();
  bool isMonotone();

  ~bool_func()
  {
  } // End of destructor
}; // end of 'bool_func' class

#endif // _BOOL_FUNC_H__
