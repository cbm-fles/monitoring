// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2021 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#include "FormatBits.hpp"

#include <algorithm>

namespace Dca {
using namespace std;

//-----------------------------------------------------------------------------
/*!
  \brief Convert uint as set-of-bits
  \param val     value to be converted
  \param bitname text for each bit
  \returns string with comma separated list of bitnames
*/

string FmtBits(uint32_t val, const vector<string>& bitname) {
  size_t nbit = min(bitname.size(), size_t(32));
  string rval;
  for (size_t i=0; i<nbit; i++) {
    if (val == 0) break;                    // bits to go ?
    if (val & 0x1) {                        // bit set ?
      if (bitname[i].size()) {              // is name defined ?
        if (rval.size()) rval += ",";
        rval += bitname[i];
      }
    }  
    val >>= 1;                              // look at next bit
  }
  return rval;
}

} // end namespace Dca
