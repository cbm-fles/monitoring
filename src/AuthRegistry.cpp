// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#include "AuthRegistry.hpp"

#include "Exception.hpp"

namespace Dca {
  
/*! \class AuthRegistry
  \brief Registry for auth tokens and rules

  Currently just an empty placeholder, to be filled with life later

  The AuthRegistry is a \glos{singleton} and accessed via the
  AuthRegistry::Ref() static method.

  \note On \ref objectownership
    - is owned by Context
*/

//-----------------------------------------------------------------------------
//! \brief Constructor

AuthRegistry::AuthRegistry()
{
  // singleton check
  if (fpSingleton)
    throw Exception("Authregistry::ctor: already instantiated");

  fpSingleton = this;
}

//-----------------------------------------------------------------------------
//! \brief Destructor

AuthRegistry::~AuthRegistry()
{
  fpSingleton = nullptr;
}

//-----------------------------------------------------------------------------
// define static member variables
AuthRegistry* AuthRegistry::fpSingleton = nullptr;

} // end namespace Dca
