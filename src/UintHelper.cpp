// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2021 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>
  
#include "UintHelper.hpp"

#include "ValueCheck.hpp"

namespace Dca {
using namespace std;

//------------------------------------------------------------------------------
/*!
  \defgroup UintHelper Helper functions for uint8_t <-> uint16_t conversions
*/

//-----------------------------------------------------------------------------
/*!
  \ingroup UintHelper
  \brief Convert uint16_t to uint8_t vector
*/

vector<uint8_t> U16ToU8(const vector<uint16_t>& vdata) {
  vector<uint8_t> rval;
  for (auto& e: vdata) {
    ValueCheck(e, 256, "U16toU8-vector");
    rval.push_back(uint8_t(e));
  }
  return rval;
}

//-----------------------------------------------------------------------------
/*!
  \ingroup UintHelper
  \brief Convert uint8_t to uint16_t vector
*/

vector<uint16_t> U8ToU16(const vector<uint8_t>& vdata) {
  vector<uint16_t> rval;
  for (auto& e: vdata) rval.push_back(e);
  return rval;
}

//-----------------------------------------------------------------------------
/*!
  \ingroup UintHelper
  \brief Convert uint16_t to uint8_t scalar
*/

uint8_t  U16ToU8(uint16_t data) {
  ValueCheck(data, 256, "U16toU8-scalar");
  return uint8_t(data);
}


} // end namespace Dca
