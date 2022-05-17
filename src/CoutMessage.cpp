// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#include "CoutMessage.hpp"

namespace Dca {
using namespace std;

/*! \class CoutMessage
  \brief Thread-safe writes to `ostream`'s, especially `cout`

  This class provides a thread-safe mechanism for writing messages to an
  `ostream`.

  Typical usage is like
  \code{.cpp}
    CoutMessage::Make().Stream()
      << "Data from " << pobj->ObjectId() << ":" << val << endl;
  \endcode
 */

//-----------------------------------------------------------------------------
/*! \brief Destructor
  Writes the message to the stream given an construction time. The execution
  of destructors is serialized via a mutex.
 */

CoutMessage::~CoutMessage() {
  lock_guard<mutex> lock(fMutex);
  fOStream << fSStream.str();
  flush(fOStream);
}

//-----------------------------------------------------------------------------
// define static member variables  
mutex CoutMessage::fMutex;

} // end namespace Dca
