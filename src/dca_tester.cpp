// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

/*!
  \file dca_tester.cpp
  \brief \glos{DCAmain} with framework test and verification \glos{DClass}es

  This \glos{DCAmain} registers the `Tester*` \glos{DClass}es provided by the
  base framework. It is mainly used for functional verification and
  performance testing of the base framework.
*/

#include "Context.hpp"

int main(int argc, char* argv[]) {
  using namespace std;
  using namespace cbm;

  // startup Dca Context
  Context dca;
  if (int rc = dca.Init(argc, argv); rc != 0) return rc;

  // do something
  return 0;
}
