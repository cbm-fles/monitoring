// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#ifndef included_Dca_Signal
#define included_Dca_Signal 1

#include <functional>
#include <vector>

// This very simple and compact Signal/Slot implementation was derived from
// Simon Schneegans very minimalist signal/slot version published under
//   https://schneegans.github.io/tutorials/2015/09/20/signal-slot
// by removing slot disconnect. Now truly minimal, just Connect() and Emit().

namespace Dca {
using namespace std;

template <typename... Args>
class Signal {
  public:
  using meth_t = function<void(Args...)>;

                    Signal() = default;

  void              Connect(function<void(Args...)>&& slot);

  template <typename T>
  void              Connect(T* pobj, void(T::* func)(Args...));
  template <typename T>
  void              Connect(T* pobj, void(T::* func)(Args...) const);

  void              Emit(Args... args);

  private:
  vector<meth_t>    fSlots {};              // list of connected slots
};

} // end namespace Dca

#include "Signal.ipp"
#endif
