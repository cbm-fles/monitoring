// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#ifndef included_Dca_ZmqHelper
#define included_Dca_ZmqHelper 1

#include "ZmqDefs.hpp"

#include <string>
#include <vector>

namespace Dca {
using namespace std;

zmsg_t String2Zmsg(const string& str);
zmsgv_t String2Zmsg(const string& str1, const string& str2);
string Zmsg2String(const zmsg_t& zmsg);

string ZmsgDump(const zmsg_t& zmsg);
string ZmsgDump(const zmsgv_t& zmsgv);

} // end namespace Dca

#include "ZmqHelper.ipp"

#endif
