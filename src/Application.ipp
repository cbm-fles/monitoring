// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020-2021 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

namespace cbm {
using namespace std;

//-----------------------------------------------------------------------------
//! \brief Returns program name

inline const string& Application::ProgName() const {
  return fProgName;
}

//-----------------------------------------------------------------------------
//! \brief Static method which returns a reference of the
//!  Application \glos{singleton}

inline Application& Application::Ref() {
  return *fpSingleton;
}

//-----------------------------------------------------------------------------
//! \brief Static method which returns a pointer to the
//!  Application \glos{singleton}

inline Application* Application::Ptr() {
  return fpSingleton;
}

} // end namespace cbm
