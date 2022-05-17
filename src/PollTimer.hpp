// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2021 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#ifndef included_Dca_PollTimer
#define included_Dca_PollTimer 1

#include "ChronoDefs.hpp"
#include "ChronoHelper.hpp"

#include <functional>

namespace Dca {
using namespace std;

class PollTimer {
  public:
  explicit          PollTimer(scduration tout);  
  void              Reset();
  void              Reset(scduration tout);
  size_t            NPoll() const;
  double            ElapsedTime() const;
                    operator bool();

  private:
  scduration        fTout {};               //!< timeout
  sctime_point      fTbeg {};               //!< start time (ctor or Reset())
  sctime_point      fTend {};               //!< end time
  sctime_point      fTnow {};               //!< time of last Poll()
  size_t            fNPoll {0};
};

bool TimedPoll(scduration tout, const function<bool()>& test);

} // end namespace Dca

#include "PollTimer.ipp"

#endif
