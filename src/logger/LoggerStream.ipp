// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

namespace cbm {

//-----------------------------------------------------------------------------
/*! \brief Returns reference of `ostream` embedded in LoggerStream

  Used in conjuction with `operator<<()` to write the message body.
 */

inline ostream& LoggerStream::Stream() {
  return fSStream;
}

} // end namespace cbm
