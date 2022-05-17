// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

namespace Dca {
using namespace std;

//-----------------------------------------------------------------------------
/*!
  \ingroup SystemHelper
  \brief Converts a `void` pointer `p` to a `uint64_t` integer

  Useful for printing pointers with fmt::format.
*/

inline uint64_t Void2Uint(void* p) {
  return uint64_t(reinterpret_cast<uintptr_t>(p));
}

//-----------------------------------------------------------------------------
/*!
  \ingroup SystemHelper
  \brief Append data to a vector<byte>
  \param  val   value be appended
  \param  bvec  vector where `val` is appended
*/

template <typename T>
inline void Append2ByteVector(T val, vector<byte>& bvec) {
     byte* pval = reinterpret_cast<byte*>(&val);
     copy(pval, pval+sizeof(val), back_inserter(bvec));
}

} // end namespace Dca
