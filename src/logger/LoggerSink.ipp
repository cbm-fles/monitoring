// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

namespace cbm {

//-----------------------------------------------------------------------------
/*! \brief Set \glos{loglevel}
  \param lvl  \glos{loglevel} for writing
 */

inline void LoggerSink::SetLogLevel(int lvl) { fLogLevel = lvl; }

//-----------------------------------------------------------------------------
//! \brief Return \glos{loglevel}

inline int LoggerSink::LogLevel() const { return fLogLevel; }

} // end namespace cbm
