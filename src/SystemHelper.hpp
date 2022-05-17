// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#ifndef included_Dca_SystemHelper
#define included_Dca_SystemHelper 1

#include <cstddef>
#include <vector>

namespace Dca {
using namespace std;

uint64_t            Void2Uint(void* p);

template <typename T>
inline void         Append2ByteVector(T val, vector<byte>& bvec);

} // end namespace Dca

#include "SystemHelper.ipp"

#endif
