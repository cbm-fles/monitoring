// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

namespace Dca {
using namespace std;

//-----------------------------------------------------------------------------
//! \ingroup ZmqHelper
//! \brief Convert a std::string into a ZMQ message

inline zmsg_t String2Zmsg(const string& str) {
  return zmsg_t(begin(str),end(str));
}
  
//-----------------------------------------------------------------------------
//! \ingroup ZmqHelper
//! \brief Convert a ZMQ message into a std::string

inline string Zmsg2String(const zmsg_t& zmsg) {
  return string(static_cast<const char*>(zmsg.data()), zmsg.size());
}
  
  
} // end namespace Dca
