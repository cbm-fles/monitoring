// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

namespace cbm {

//-----------------------------------------------------------------------------
//! \brief Returns hostname used by Logger

inline const string& Logger::HostName() const {
  return fHostName;
}

//-----------------------------------------------------------------------------
//! \brief Returns program name used by Logger

inline const string& Logger::ProgName() const {
  return fProgName;
}

//-----------------------------------------------------------------------------
//! \brief Static method which returns a reference of the
//!  Logger \glos{singleton}

inline Logger& Logger::Ref() {
  return *fpSingleton;
}

//-----------------------------------------------------------------------------
//! \brief Static method which returns a pointer to the
//!  Logger \glos{singleton}

inline Logger* Logger::Ptr() {
  return fpSingleton;
}

} // end namespace cbm

