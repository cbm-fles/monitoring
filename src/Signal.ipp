// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

namespace Dca {

//-----------------------------------------------------------------------------
template <typename... Args>
inline void Signal<Args...>::Connect(function<void(Args...)>&& slot) {
  fSlots.push_back(move(slot));
}

//-----------------------------------------------------------------------------
template <typename... Args>
template <typename T>
inline void Signal<Args...>::Connect(T* pobj, void(T::* func)(Args...)) {
  Connect([=](Args... args) {(pobj->*func)(args...);});
}

//-----------------------------------------------------------------------------
template <typename... Args>
template <typename T>
inline void Signal<Args...>::Connect(T* pobj, void(T::* func)(Args...) const) {
  Connect([=](Args... args) {(pobj->*func)(args...);});
}

//-----------------------------------------------------------------------------
template <typename... Args>
inline void Signal<Args...>::Emit(Args... args){
  for (auto& slot : fSlots) slot(forward<Args>(args)...);
}
  
} // end namespace Dca
