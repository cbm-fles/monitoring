// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2021 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#ifndef included_Dca_ValueCheck
#define included_Dca_ValueCheck 1

#include <cstdint>

namespace Dca {
using namespace std;
  
void  ValueCheck(uint64_t val, uint64_t size, const char* ptxt);
void  ValueCheck(uint64_t val, uint64_t min, uint64_t max, const char* ptxt);
void  ValueCheck(int64_t val, int64_t min, int64_t max, const char* ptxt);
void  ValueCheck(double val, double min, double max, const char* ptxt);

} // end namespace Dca

//#include "ValueCheck.ipp"

#endif
