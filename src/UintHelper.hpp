// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2021 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#ifndef included_Dca_UintHelper
#define included_Dca_UintHelper 1

#include <cstdint>
#include <vector>

namespace Dca {
using namespace std;

vector<uint8_t>  U16ToU8(const vector<uint16_t>& vdata);
vector<uint16_t> U8ToU16(const vector<uint8_t>& vdata);

uint8_t    U16ToU8(uint16_t data);
uint16_t   U8ToU16(uint8_t data);

uint8_t    U16Lsb(uint16_t dat);
uint8_t    U16Msb(uint16_t dat);
uint16_t   U16Pack(uint8_t msb, uint8_t lsb);

uint16_t   U32Lsb(uint32_t dat);
uint16_t   U32Msb(uint32_t dat);
uint32_t   U32Pack(uint16_t msb, uint16_t lsb);

uint32_t   U64Lsb(uint64_t dat);
uint32_t   U64Msb(uint64_t dat);
uint64_t   U64Pack(uint32_t msb, uint32_t lsb);

} // end namespace Dca

#include "UintHelper.ipp"

#endif
