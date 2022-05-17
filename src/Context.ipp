// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020-2021 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

namespace Dca {
using namespace std;

//-----------------------------------------------------------------------------
//! \brief Returns program name

inline const string& Context::ProgName() const {
  return fProgName;
}

//-----------------------------------------------------------------------------
//! \brief Static method which returns a reference of the
//!  Context \glos{singleton}

inline Context& Context::Ref() {
  return *fpSingleton;
}

//-----------------------------------------------------------------------------
//! \brief Static method which returns a pointer to the
//!  Context \glos{singleton}

inline Context* Context::Ptr() {
  return fpSingleton;
}

} // end namespace Dca
