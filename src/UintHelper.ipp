// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2021 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

namespace Dca {
using namespace std;

//-----------------------------------------------------------------------------
//! \ingroup UintHelper
//! \brief convert uint8_t to uint16_t scalar

inline uint16_t U8ToU16(uint8_t data) {
  return uint16_t(data);
}  

//-----------------------------------------------------------------------------
//! \ingroup UintHelper
//! \brief Returns lower 8bit of 16bit word

inline uint8_t U16Lsb(uint16_t dat) {
  return uint8_t(dat);
}

//-----------------------------------------------------------------------------
//! \ingroup UintHelper
//! \brief Returns upper 8bit of 16bit word

inline uint8_t U16Msb(uint16_t dat) {
  return uint8_t(dat>>8);
}

//-----------------------------------------------------------------------------
//! \ingroup UintHelper
//! \brief Returns 16bit word packed from 8bit `msb` and `lsb` parts

inline uint16_t U16Pack(uint8_t msb, uint8_t lsb) {
  return  uint16_t((uint16_t(msb)<<8) | uint16_t(lsb));
}

//-----------------------------------------------------------------------------
//! \ingroup UintHelper
//! \brief Returns lower 16bit of 32bit word

inline uint16_t U32Lsb(uint32_t dat) {
  return uint16_t(dat);
}

//-----------------------------------------------------------------------------
//! \ingroup UintHelper
//! \brief Returns upper 16bit of 32bit word

inline uint16_t U32Msb(uint32_t dat) {
  return uint16_t(dat>>16);
}

//-----------------------------------------------------------------------------
//! \ingroup UintHelper
//! \brief Returns 32bit word packed from 16bit `msb` and `lsb` parts

inline uint32_t U32Pack(uint16_t msb, uint16_t lsb) {
  return  uint32_t((uint32_t(msb)<<16) | uint32_t(lsb));
}

//-----------------------------------------------------------------------------
//! \ingroup UintHelper
//! \brief Returns lower 32bit of 64bit word

inline uint32_t U64Lsb(uint64_t dat) {
  return uint32_t(dat);
}

//-----------------------------------------------------------------------------
//! \ingroup UintHelper
//! \brief Returns upper 32bit of 64bit word

inline uint32_t U64Msb(uint64_t dat) {
  return uint32_t(dat>>32);
}

//-----------------------------------------------------------------------------
//! \ingroup UintHelper
//! \brief Returns 64bit word packed from 32bit `msb` and `lsb` parts

inline uint64_t U64Pack(uint32_t msb, uint32_t lsb) {
  return  uint64_t((uint64_t(msb)<<32) | uint64_t(lsb));
}
  
} // end namespace Dca
