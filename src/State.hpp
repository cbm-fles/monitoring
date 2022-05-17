// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#ifndef included_Dca_State
#define included_Dca_State 1

#include <cstdint>

namespace Dca {

struct State {
                    bool operator==(const State& rhs) const;
                    bool operator<(const State& rhs) const;
  uint16_t          fMajor {kMaNewed};      //!< major state
  uint16_t          fMinor {kMiNone};       //!< minor state

  // some constants
  static const uint16_t kMaNewed=0;
  static const uint16_t kMaConfiguring=1;
  static const uint16_t kMaDisabled=2;
  static const uint16_t kMaEnabled=3;
  static const uint16_t kMaInitializing=4;
  static const uint16_t kMaInactive=5;
  static const uint16_t kMaActive=6;
  static const uint16_t kMaFailed=7;

  static const uint16_t kMiNone=0;
  static const uint16_t kMiFromConfiguring=kMaConfiguring;
  static const uint16_t kMiFromEnabled=kMaEnabled;
  static const uint16_t kMiFromDisabled=kMaDisabled;
  static const uint16_t kMiFromInitializing=kMaInitializing;
  static const uint16_t kMiFromActive=kMaActive;
  static const uint16_t kMiFromInactive=kMaInactive;
  static const uint16_t kMiFromFailed=kMaFailed;
};

  
} // end namespace Dca

#include "State.ipp"

#endif
