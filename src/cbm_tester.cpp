// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

/*!
  \file cbm_tester.cpp
  \brief \glos{CBMmain} with framework test and verification \glos{DClass}es

  This \glos{CBMmain} registers the `Tester*` \glos{DClass}es provided by the
  base framework. It is mainly used for functional verification and
  performance testing of the base framework.
*/

#include "Application.hpp"

int main(int argc, char* argv[]) {
  using namespace std;
  using namespace cbm;

  // startup Cbm Application
  Application cbm;
  if (int rc = cbm.Init(argc, argv); rc != 0) return rc;

  // do something
  return 0;
}
