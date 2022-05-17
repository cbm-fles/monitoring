// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

namespace Dca {

/*! \class State
  \brief Represents the state of a \glos{DObject}.

  The state has two components
  - `fMajor`
  - `fMinor`
 */

//-----------------------------------------------------------------------------
//! \brief Equal compare operator.
inline bool State::operator==(const State& rhs) const {
  return fMajor==rhs.fMajor && fMinor==rhs.fMinor;
}

//-----------------------------------------------------------------------------
//! \brief Less compare operator.
/*!
  Sorts first by `fMajor`, and when they are equal, by `fMinor`.
  This operator allows to use `State` as key in a `std::map`.
 */
inline bool State::operator<(const State& rhs) const {
  if (fMajor == rhs.fMajor) return fMinor<rhs.fMinor;
  return fMajor<rhs.fMajor;
}

} // end namespace Dca
