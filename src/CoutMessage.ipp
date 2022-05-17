// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

namespace Dca {

//-----------------------------------------------------------------------------
//! \brief Constructor, will bind to `cout` stream

inline CoutMessage::CoutMessage() :
  fOStream(cout)
{}    

//-----------------------------------------------------------------------------
//! \brief Constructor, will bind to `os` stream

inline CoutMessage::CoutMessage(ostream& os) :
  fOStream(os)
{}

//-----------------------------------------------------------------------------
//! \brief Returns reference to message buffer stream

inline ostream& CoutMessage::Stream() {
  return fSStream;
}

//-----------------------------------------------------------------------------
//! \brief Creates CoutMessage object bound to `cout` stream

inline CoutMessage CoutMessage::Make() {
  return CoutMessage();
}

//-----------------------------------------------------------------------------
//! \brief Creates CoutMessage object bound to `os` stream

inline CoutMessage CoutMessage::Make(ostream& os) {
  return CoutMessage(os);
}

//-----------------------------------------------------------------------------
template <class T>
inline ostream& operator<<(CoutMessage& cmsg, const T& val) {
  cmsg.Stream() << val;
  return cmsg.Stream();
} 

} // end namespace Dca
