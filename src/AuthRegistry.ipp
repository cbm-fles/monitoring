// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

namespace Dca {

//-----------------------------------------------------------------------------
//! \brief Static method which returns a reference of the
//! AuthRegistry \glos{singleton}

inline AuthRegistry& AuthRegistry::Ref() {
  return *fpSingleton;
}

//-----------------------------------------------------------------------------
//! \brief Static method which returns a pointer to the
//! AuthRegistry \glos{singleton}

inline AuthRegistry* AuthRegistry::Ptr() {
  return fpSingleton;
}
    
} // end namespace Dca

