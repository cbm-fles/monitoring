// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2021 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

namespace Dca {
using namespace std;

/*! \class PollTimer
  \brief Timer for poll loop timeouts

  Typical use is in a method is
  \code{.cpp}
  PollTimer timer(1ms);
  while (timer) {
    if (... check poll condition ...) return ... success ...;
  }
  throw Exception(.... failure ....);
  \endcode
 */

//-----------------------------------------------------------------------------
/*! \brief Constructor
  \param tout timeout duration
 */

inline PollTimer::PollTimer(scduration tout) :
  fTout(tout)
{
  Reset();
}
  
//-----------------------------------------------------------------------------
//! \brief Restart timer with timeout specified in constructor

inline void PollTimer::Reset() {
  fTbeg = ScNow();
  fTend = fTbeg + fTout;
  fTnow = fTbeg;
  fNPoll = 0;
}

//-----------------------------------------------------------------------------
//! \brief Restart timer with new timeout

inline void PollTimer::Reset(scduration tout) {
  fTout = tout;
  Reset();
}

//-----------------------------------------------------------------------------
//! \brief Returns number of polls

inline size_t PollTimer::NPoll() const {
  return fNPoll;
}

//-----------------------------------------------------------------------------
//! \brief Returns time from ctor or Reset() till last poll

inline double PollTimer::ElapsedTime() const {
  return ScTimeDiff2Double(fTbeg, fTnow);
}

//-----------------------------------------------------------------------------
//! \brief Returns `false` if timer expired or measures time of current poll

inline PollTimer::operator bool() {
  if (fTnow > fTend) return false;
  fTnow = chrono::system_clock::now();
  fNPoll += 1;
  return true;
}

//-----------------------------------------------------------------------------
/*!
  \brief Poll predicate `test` for at most `tout`
  \param tout   maximal poll time
  \param test   predicate, poll tested till `true`
  \returns `true` if `test()` is true before `tout`, `false` otherwise
*/

inline bool TimedPoll(scduration tout, const function<bool()>& test) {
  PollTimer timer(tout);
  while (timer) {
    if (test()) return true;
  }
  return false;
}


} // end namespace Dca
