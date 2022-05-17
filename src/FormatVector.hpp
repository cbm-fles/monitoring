// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2021 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#ifndef included_Dca_FormatVector
#define included_Dca_FormatVector 1

#include <string>
#include <vector>

namespace Dca {
using namespace std;

template <typename T>
string FmtVec(const string& fmt, vector<T> vec, int maxfmt=0);

} // end namespace Dca

#include "FormatVector.ipp"

#endif
