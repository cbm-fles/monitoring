// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#ifndef included_Dca_SysCallException
#define included_Dca_SysCallException 1

# include "Exception.hpp"

namespace Dca {
using namespace std;

class SysCallException : public Exception {
  public:
                    SysCallException(const string& where, const string& call,
                                     int eno);
                    SysCallException(const string& where, const string& call,
                                     const string& info, int eno);
};
  
} // end namespace Dca

//#include "SysCallException.ipp"

#endif
