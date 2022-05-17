// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#ifndef included_Dca_BuildInfo
#define included_Dca_BuildInfo 1

#include <string>
#include <unordered_map>

namespace Dca {
using namespace std;

unordered_map<string,string> BuildInfo();
  
} // end namespace Dca

#endif
