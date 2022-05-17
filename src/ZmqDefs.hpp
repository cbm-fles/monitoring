// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#ifndef included_Dca_ZmqDefs
#define included_Dca_ZmqDefs 1

#include <zmq.hpp>

#include <vector>

namespace Dca {

using zmsg_t  = zmq::message_t;
using zmsgv_t = std::vector<zmsg_t>;
using zsock_t = zmq::socket_t;
using zcntx_t = zmq::context_t;

} // end namespace Dca

#endif
