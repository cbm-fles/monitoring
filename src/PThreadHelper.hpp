// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#ifndef included_Dca_PThreadHelper
#define included_Dca_PThreadHelper 1

#include <string>

namespace Dca {
using namespace std;

void SetPThreadName(const string& name);
string PThreadName();

} // end namespace Dca

//#include "PThreadHelper.ipp"

#endif
