// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#ifndef included_Dca_AuthRegistry
#define included_Dca_AuthRegistry 1

namespace Dca {

class AuthRegistry {
  public:

                    AuthRegistry();
  virtual           ~AuthRegistry();
  
                    AuthRegistry(const AuthRegistry&) = delete;
                    AuthRegistry& operator=(const AuthRegistry&) = delete;

  static AuthRegistry& Ref();
  static AuthRegistry* Ptr();

  private:
  static AuthRegistry* fpSingleton;         //!< \glos{singleton} this
};
    
} // end namespace Dca

#include "AuthRegistry.ipp"

#endif
