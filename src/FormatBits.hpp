// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2021 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#ifndef included_Dca_FormatBits
#define included_Dca_FormatBits 1

#include <cstdint>
#include <string>
#include <vector>

namespace Dca {
using namespace std;

string FmtBits(uint32_t val, const vector<string>& bitname);

} // end namespace Dca

//#include "FormatBits.ipp"

#endif
