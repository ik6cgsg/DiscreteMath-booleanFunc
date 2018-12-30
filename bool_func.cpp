#include <iomanip>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>

#include "bool_func.h"

// I/O methods for truth table

std::istream & operator>>(std::istream & in, bool_func & bf)
{
  std::string str;
  std::getline(in, str);

  bf.n = (byte)log2(str.size() << 2);

  bf.TVT.clear();
  bf.PDNF.clear();
  bf.PCNF.clear();
  bf.RDNF.clear();
  bf.ZheP = uvec((((1 << bf.n) - 1) >> 5) + 1);
  bf.CF.clear();
  bf.DF.clear();
  bf.Karn = umatr(1 << (bf.n >> 1));
  for (auto i = bf.Karn.begin(); i != bf.Karn.end(); i++)
    *i = uvec(((1 << ((bf.n >> 1) + (bf.n & 1))) >> 5) + 1);

  while (!str.empty())
  {
    int size = str.size();

    if (size > 8)
      size = 8;

    std::stringstream sts(str.substr(0, size));
    str = str.substr(size);

    uint tmp;
    sts >> std::hex >> tmp;
    bf.TVT.push_back(tmp);
  }
  bf.convers(tTVT, std::vector<rep_type>({tPDNF, tPCNF, tZheP, tKarn}), true);

  return in;
} // End of 'operator>>' function

std::ostream & operator<<(std::ostream & out, bool_func & bf)
{
  int w = (1 << bf.n) >> 2;
  bf.print(out, w, bf.TVT);
  return out;
} // End of 'operator<<' function

void bool_func::read(std::istream & in, rep_type tp, luint m)
{
  if (tp == tTVT)
    return (void)operator>>(in, *this);

  std::string str;
  std::getline(in, str);

  TVT.clear();
  PDNF.clear();
  PCNF.clear();
  RDNF.clear();
  ZheP.clear();
  CF.clear();
  DF.clear();
  Karn.clear();

  // getting n
  switch (tp)
  {
  case tZheP:
    n = (byte)log2(str.size() << 2);
    break;
  case tPDNF:
    mPDNF = m;
    n = (byte)((str.size() << 2) / m);
    break;
  case tPCNF:
    mPCNF = m;
    n = (byte)((str.size() << 2) / m);
    break;
  case tRDNF:
    mRDNF = m;
    n = (byte)((str.size() << 1) / m);
    break;
  case tCF:
    mCF = m;
    n = (byte)((str.size() << 1) / m);
    break;
  case tDF:
    mDF = m;
    n = (byte)((str.size() << 1) / m);
    break;
  case tKarn:
    n = m;
    Karn = umatr(1 << (n >> 1));
    break;
  }

  while (!str.empty())
  {
    int size = str.size();

    if (size > 8)
      size = 8;

    std::stringstream sts(str.substr(0, size));
    str = str.substr(size);

    uint tmp;
    sts >> std::hex >> tmp;

    switch (tp)
    {
    case tZheP:
      ZheP.push_back(tmp);
      break;
    case tPDNF:
      PDNF.push_back(tmp);
      break;
    case tPCNF:
      PCNF.push_back(tmp);
      break;
    case tRDNF:
      RDNF.push_back(tmp);
      break;
    case tCF:
      CF.push_back(tmp);
      break;
    case tDF:
      DF.push_back(tmp);
      break;
    case tKarn:
      Karn[0].push_back(tmp);
      break;
    }
  }

  if (tp == tKarn)
    for (int i = 1; std::getline(in, str), i < 1 << (n >> 1); i++)
      while (!str.empty())
      {
        int size = str.size();
        if (size > 8)
          size = 8;

        std::stringstream sts(str.substr(0, size));
        str = str.substr(size);
        uint tmp;
        sts >> std::hex >> tmp;
        Karn[i].push_back(tmp);
      }

  switch (tp)
  {
  case tZheP:
    *this = bool_func(n, ZheP, tp);
    return;
  case tPDNF:
    *this = bool_func(n, PDNF, tp, m);
    return;
  case tPCNF:
    *this = bool_func(n, PCNF, tp, m);
    return;
  case tRDNF:
    *this = bool_func(n, RDNF, tp, m);
    return;
  case tCF:
    *this = bool_func(n, CF, tp, m);
    return;
  case tDF:
    *this = bool_func(n, DF, tp, m);
    return;
  case tKarn:
    *this = bool_func(n, Karn);
    return;
  }
} // End of 'bool_func::read' function

void bool_func::write(std::ostream & out, rep_type tp)
{
  int w;

  switch (tp)
  {
  case tTVT:
    operator<<(out, *this);
    break;
  case tPDNF:
    w = mPDNF * n >> 2;
    print(out, w, PDNF);
    break;
  case tPCNF:
    w = mPCNF * n >> 2;
    print(out, w, PCNF);
    break;
  case tDF:
    w = mDF * n >> 1;
    print(out, w, DF);
    break;
  case tCF:
    w = mCF * n >> 1;
    print(out, w, CF);
    break;
  case tRDNF:
    w = mRDNF * n >> 1;
    print(out, w, RDNF);
    break;
  case tZheP:
    w = 1 << n - 2;
    print(out, w, ZheP);
    break;
  case tKarn:
    w = 1 << ((n >> 1) + (n & 1)) - 2;
    for (uint i = 0; i < Karn.size(); i++)
      print(out, w, Karn[i]);
    break;
  }
} // End of 'bool_func::write' function

bool bool_func::value(uint x, rep_type tp)
{
  uint sum = 0, bits, xorMask = (1u << n) - 1;

  switch (tp)
  {
  case tTVT:
  case tPDNF:
  case tPCNF:
    return TVT[TVT.size() - 1 - (x >> 5)] >> (x & 0x1f) & 1;
  case tZheP:
    bits = n > 5 ? 0x1f : (1 << n) - 1;
    uint set, wZind, bZind, bZmask;
    for (set = 0; wZind = set >> 5, bZind = set & 0x1f, bZmask = 1u << bits - bZind, set < 1u << n; set++)
      sum ^= (ZheP[wZind] & bZmask) == bZmask && (set & x) == set;
    return sum;
  case tRDNF:
    for (uint i = 0; i < mRDNF; i++)
      if (((getSet(mRDNF + i, RDNF, mRDNF * 2) ^ xorMask ^ x) & getSet(i, RDNF, mRDNF * 2)) == getSet(i, RDNF, mRDNF * 2))
        return true;
    return false;
  case tDF:
    for (uint i = 0; i < mDF; i++)
      if (((getSet(mDF + i, DF, mDF * 2) ^ xorMask ^ x) & getSet(i, DF, mDF * 2)) == getSet(i, DF, mDF * 2))
        return true;
    return false;
  case tCF:
    for (uint i = 0; i < mCF; i++)
      if ((getSet(mCF + i, CF, mCF * 2) ^ x) & getSet(i, CF, mCF * 2) == 0)
        return 0;
    return 1;
  case tKarn:
    uint s = Karn[0].size();
    bits = 1 << ((n >> 1) + (n & 1));
    uint i = Grey2Int(x >> n - (n >> 1)), j = Grey2Int(x & (1 << n - (n >> 1)) - 1), wInd = s - 1 - (j >> 5), bMask = bits - 1 - (j & 0x1f);
    return Karn[i][wInd] >> bMask & 1;
  }

  return false;
} // End of 'bool_func::value' function

bool bool_func::saveZero()
{
  return value(0) == false;
} // End of 'bool_func::saveZero' function

bool bool_func::saveOne()
{
  return value((1 << n) - 1) == true;
} // End of 'bool_func::saveOne' function

bool bool_func::isSelfDual()
{
  uint max = n < 32 ? (1u << n) - 1 : 0xFFFFFFFF;
  for (uint x = 0; x < 1u << n; x++)
    if (value(x) != ~value(x ^ max))
      return false;
  return true;
} // End of 'bool_func::isSelfDual' function

bool bool_func::isLinear()
{
  uint bits = n > 5 ? 0x1f : (1 << n) - 1;
  uint x, wZind, bZind, bZmask;
  for (x = 0; wZind = x >> 5, bZind = x & 0x1f, bZmask = 1u << bits - bZind, x < 1u << n; x++)
    if ((ZheP[wZind] & bZmask) == bZmask && !(x & (x - 1)))     // deg(ZheP) > 1
      return false;
  return true;
} // End of 'bool_func::isLinear' function

bool bool_func::isMonotone()
{
  for (uint x = 0; x < 1u << n; x++)
    for (uint y = x; y < 1u << n; y++)
      if ((x | y) == y && value(x) > value(y))  // ![(x <= y) => (f(x) <= f(y))] == (x <= y) & f(x) > f(y)
        return false;
  return true;
} // End of 'bool_func::isMonotone' function

void bool_func::convers(rep_type typeFrom, std::vector<rep_type> typesTo, bool needQuine)
{
  if (typeFrom != tTVT)
    toTVT(typeFrom);

  for (uint i = 0; i < typesTo.size(); i++)
    fromTVT(typesTo[i]), typesTo[i] == tPDNF && needQuine ? Quine() : (void)nullptr;
} // End of 'bool_func::init' function

void bool_func::pack(uvec inv, rep_type tp, uvec ent)
{
  int size = inv.size();
  uint bits = n * size, wInd;
  bits = bits > 0x20 ? 0x20 : bits;
  int bInd;

  switch (tp)
  {
  case tPDNF:
    PDNF = uvec((size * n >> 5) + 1);

    for (uint i = 0; wInd = i * n >> 5, bInd = bits - (i * n & 0x1f) - n, i < size; i++)
      if (bInd >= 0)
        PDNF[wInd] ^= inv[i] << bInd;
      else
        PDNF[wInd] ^= inv[i] >> (-bInd), PDNF[wInd + 1] ^= (inv[i] & ((1 << (-bInd)) - 1)) << bits + bInd;
    break;
  case tPCNF:
    PCNF = uvec((size * n >> 5) + 1);

    for (uint i = 0; wInd = i * n >> 5, bInd = bits - (i * n & 0x1f) - n, i < size; i++)
      if (bInd >= 0)
        PCNF[wInd] ^= inv[i] << bInd;
      else
        PCNF[wInd] ^= inv[i] >> (-bInd), PCNF[wInd + 1] ^= (inv[i] & ((1 << (-bInd)) - 1)) << bits + bInd;
    break;
  case tRDNF:
    RDNF = uvec((size * 2 * n >> 5) + 1);
    bits = bits * 2 > 0x20 ? 0x20 : bits * 2;
    uint ind;
    uvec *ptr;

    for (uint i = 0; ind = i >= size ? i - size : i, ptr = i >= size ? &inv : &ent, wInd = i * n >> 5, bInd = bits - (i * n & 0x1f) - n, i < size * 2; i++)
      if (bInd >= 0)
        RDNF[wInd] ^= (*ptr)[ind] << bInd;
      else
        RDNF[wInd] ^= (*ptr)[ind] >> (-bInd), RDNF[wInd + 1] ^= ((*ptr)[ind] & ((1 << (-bInd)) - 1)) << bits + bInd;
    break;
  }
} // End of 'bool_func::pack' function

uint bool_func::getSet(uint i, uvec bf, luint m)
{
  uint bits = n * m, wInd = i * n >> 5, max = n < 32 ? (1 << n) - 1 : 0xFFFFFFFF;
  bits = bits > 0x20 ? 0x20 : bits;
  int bInd = bits - (i * n & 0x1f) - n;
  return bInd >= 0 ? bf[wInd] >> bInd & max : bf[wInd] << (-bInd) & max | bf[wInd + 1] >> bits + bInd;
} // End of 'bool_func::getSet' function

void bool_func::print(std::ostream & out, int w, uvec bf)
{
  out << std::hex << std::uppercase << std::setfill('0') << std::setw(w > 8 ? 8 : w) << bf[0];
  for (uint i = 1; i < bf.size(); i++)
    out << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << bf[i];
  out << std::endl;
} // End of 'bool_func::print' function

bool bool_func::consistPair(uvec & entV, uvec & invV, uint ent, uint inv)
{
  auto itInv = std::find(invV.begin(), invV.end(), inv);

  while (itInv != invV.end())
  {
    int ind = itInv - invV.begin();
    if (entV[ind] == ent)
      return true;
    itInv = std::find(itInv + 1, invV.end(), inv);
  }

  return false;
} // End of 'bool_func::consistPair' function

// For TVT
void bool_func::setValue(uint x, bool val)
{
  uint wTind = TVT.size() - 1 - (x >> 5), bTind = x & 0x1f, bTmask = 1 << bTind;
  TVT[wTind] ^= (TVT[wTind] & bTmask) == bTmask != val ? bTmask : 0;
} // End of 'bool_func::setValue' function

/// Convertors

void bool_func::fromTVT(rep_type tp)
{
  uint bits;
  switch (tp)
  {
  case tPDNF:
    for (int i = 0; i < 1 << n; i++)
      if (value(i))
        PDNF.push_back(i);
    mPDNF = PDNF.size();
    pack(PDNF, tp);
    break;
  case tPCNF:
    for (int i = 0; i < 1 << n; i++)
      if (!value(i))
        PCNF.push_back(i);
    mPCNF = PCNF.size();
    pack(PCNF, tp);
    break;
  case tKarn:
    uint x, i, j, s, bMask, wInd;
    bits = 1u << ((n >> 1) + (n & 1));
    bits = bits > 0x20 ? 0x20 : bits;
    s = Karn[0].size();
    for (x = 0; i = Grey2Int(x >> n - (n >> 1)), j = Grey2Int(x & (1 << (n - (n >> 1))) - 1), wInd = s - 1 - (j >> 5), bMask = 1 << bits - 1 - (j & 0x1f), x < 1u << n; x++)
      Karn[i][wInd] ^= ((Karn[i][wInd] & bMask) == bMask) != value(x) ? bMask : 0;
    x = 4;
    break;
  case tZheP:
    uint D = 1u << n, wZind, bZind, bZmask, wTind, bTind, bTmask;
    bits = n > 5 ? 0x1f : (1u << n) - 1;
    for (uint d = 0; wZind = d >> 5, bZind = d & 0x1f, bZmask = 1u << bits - bZind, d < D; d++)
      for (uint x = 0; wTind = TVT.size() - 1 - (x >> 5), bTind = x & 0x1f, bTmask = 1u << bTind, x <= d; x++)
        ZheP[wZind] ^= (TVT[wTind] & bTmask) == bTmask && x == (x & d) ? bZmask : 0;
    D = 4;
    break;
  }
} // End of 'bool_func::fromTVT' function

void bool_func::toTVT(rep_type tp)
{
  switch (tp)
  {
  case tPDNF:
    for (int i = 0; i < 1 << n; i++)
      setValue(i, false);
    for (uint x, i = 0; i < mPDNF; i++)
      x = getSet(i, PDNF, mPDNF), setValue(x, true);
    break;
  case tPCNF:
    for (int i = 0; i < 1 << n; i++)
      setValue(i, true);
    for (uint x, i = 0; i < mPCNF; i++)
      x = getSet(i, PCNF, mPCNF), setValue(x, false);
    break;
  case tZheP:
  case tRDNF:
  case tDF:
  case tCF:
  case tKarn:
    for (int i = 0; i < 1 << n; i++)
      setValue(i, value(i, tp));
    break;
  }
} // End of 'bool_func::toTVT' function

void bool_func::Quine()
{
  uvec ent, inv, entF, invF, entUsed, invUsed;
  uint max = n < 32 ? (1u << n) - 1 : 0xFFFFFFFF, v;

  for (uint i = 0; i < 1u << n; i++)
    if (value(i))
      inv.push_back(i);

  for (uint i = 0; i < inv.size(); i++)
    ent.push_back(max);

  for (uint j = 0; j < inv.size(); j++)
  {
    uint size = ent.size();
    for (uint k = j + 1; k < size; k++)
      if ((v = ent[j] == ent[k] ? inv[j] ^ inv[k] : 0) && !(v & (v - 1)))
        ent.push_back((v ^ max) & ent[j]), inv.push_back(inv[j]), entUsed.push_back(ent[k]), entUsed.push_back(ent[j]), invUsed.push_back(inv[k]), invUsed.push_back(inv[j]);

    if (size == ent.size() && !consistPair(entUsed, invUsed, ent[j], inv[j]) && !consistPair(entF, invF, ent[j], inv[j]))
      entF.push_back(ent[j]), invF.push_back(inv[j]);
  }
  mRDNF = invF.size();
  pack(invF, tRDNF, entF);
  int i = 0;
} // End of 'bool_func::Quine' function

// From index to Grey code
uint bool_func::Grey(uint ind)
{
  return ind ^ ind >> 1;
} // End of 'bool_func::Grey' function

// From Grey code to index
uint bool_func::Grey2Int(uint g)
{
  uint ind, m, b; // Most Significant Bit xOR-Accumulator

  for (m = 1U << 31; ~(g & m) & m; m >>= 1); // Most Significant Bit of G
  for (ind = b = 0; m; b ^= g & m, ind ^= b, b >>= 1, m >>= 1);

  return ind;
} // End of 'bool_func::Grey2Int' function

bool_func::bool_func(byte n, const uvec & source, rep_type tp, luint m)
{
  this->n = n;

  TVT = uvec((((1 << n) - 1) >> 5) + 1);
  PDNF.clear();
  PCNF.clear();
  RDNF.clear();
  ZheP = uvec((((1 << n) - 1) >> 5) + 1);
  CF.clear();
  DF.clear();
  Karn = umatr(1 << (n >> 1));
  for (auto i = Karn.begin(); i != Karn.end(); i++)
    *i = uvec(((1 << ((n >> 1) + (n & 1))) >> 5) + 1);

  switch (tp)
  {
  case tTVT:
    TVT = source;
    convers(tp, std::vector<rep_type>({tPDNF, tPCNF, tZheP, tKarn}), true);
    break;
  case tPDNF:
    PDNF = source;
    mPDNF = m;
    convers(tp, std::vector<rep_type>({tPCNF, tZheP, tKarn}), false);
    Quine(); // PDNF -> RDNF
    break;
  case tPCNF:
    PCNF = source;
    mPCNF = m;
    convers(tp, std::vector<rep_type>({tPDNF, tZheP, tKarn}), true);
    break;
  case tZheP:
    ZheP = source;
    convers(tp, std::vector<rep_type>({tPDNF, tPCNF, tKarn}), true);
    break;
  case tRDNF:
    RDNF = source;
    mRDNF = m;
    convers(tp, std::vector<rep_type>({tPDNF, tPCNF, tZheP, tKarn}), false);
    break;
  case tDF:
    DF = source;
    mDF = m;
    convers(tp, std::vector<rep_type>({tPDNF, tPCNF, tZheP, tKarn}), true);
    break;
  case tCF:
    CF = source;
    mCF = m;
    convers(tp, std::vector<rep_type>({tPDNF, tPCNF, tZheP, tKarn}), true);
    break;
  }
} // End of constructor

bool_func::bool_func(byte n, const umatr & KarnMatr)
{
  this->n = n;
  TVT = uvec((((1 << n) - 1) >> 5) + 1);
  PDNF.clear();
  PCNF.clear();
  RDNF.clear();
  ZheP = uvec((((1 << n) - 1) >> 5) + 1);
  Karn = KarnMatr;
  convers(tKarn, std::vector<rep_type>({tPDNF, tPCNF, tZheP}), true);
} // End of constructor
