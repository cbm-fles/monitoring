// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2021 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#include "ValueCheck.hpp"

#include "Exception.hpp"

#include "fmt/format.h"

namespace Dca {
using namespace std;

//------------------------------------------------------------------------------
/*!
  \defgroup ValueCheck Helper functions
*/

//-----------------------------------------------------------------------------
/*!
  \ingroup ValueCheck
  \brief Checks that unsigned integer `val` is < `size`
  \throws Exception if `val` >= `size`, use `ptxt` as descriptive text
*/

void ValueCheck(uint64_t val, uint64_t size, const char* ptxt) {
  if (val >= size)
    throw Exception(fmt::format("{}: out-of-range, got={:#x}, size={:#x}",
                                ptxt, val, size));
}

//-----------------------------------------------------------------------------
/*!
  \ingroup ValueCheck
  \brief Checks that unsigned integer `val` is > `min` and <= `max`
  \throws Exception if `val` not in [`min`,`max`], use `ptxt` as descriptive text
*/

void ValueCheck(uint64_t val, uint64_t min, uint64_t max, const char* ptxt) {
  if (val < min || val > max)
    throw Exception(fmt::format("{}: out-of-range,"
                                " got={:#x}, min={:#x}, max={:#x}",
                                ptxt, val, min, max));
}

//-----------------------------------------------------------------------------
/*!
  \ingroup ValueCheck
  \brief Checks that signed integer `val` is > `min` and <= `max`
  \throws Exception if `val` not in [`min`,`max`], use `ptxt` as descriptive text
*/

void ValueCheck(int64_t val, int64_t min, int64_t max, const char* ptxt) {
  if (val < min || val > max)
    throw Exception(fmt::format("{}: out-of-range, got={}, min={}, max={}",
                                ptxt, val, min, max));
}

//-----------------------------------------------------------------------------
/*!
  \ingroup ValueCheck
  \brief Checks that double `val` is > `min` and <= `max`
  \throws Exception if `val` not in [`min`,`max`], use `ptxt` as descriptive text
*/

void ValueCheck(double val, double min, double max, const char* ptxt) {
  if (val < min || val > max)
    throw Exception(fmt::format("{}: out-of-range, got={}, min={}, max={}",
                                ptxt, val, min, max));
}


} // end namespace Dca
