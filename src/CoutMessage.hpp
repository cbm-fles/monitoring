// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#ifndef included_Dca_CoutMessage
#define included_Dca_CoutMessage 1

#include <iostream>
#include <mutex>
#include <sstream>

namespace Dca {
using namespace std;

class CoutMessage {
  public:
                    CoutMessage();
  explicit          CoutMessage(ostream& os);
                    ~CoutMessage();

  ostream&          Stream();

  static CoutMessage Make();
  static CoutMessage Make(ostream& os);

  private:
  ostream&          fOStream;               //!< reference to output stream
  ostringstream     fSStream;               //!< stringstream for message
  static mutex      fMutex;                 //!< mutex to protect stream write
};

template <class T>
ostream& operator<<(CoutMessage& cmsg, const T& val);

} // end namespace Dca

#include "CoutMessage.ipp"

#endif
