// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>
  
#include "ZmqHelper.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>

namespace Dca {
using namespace std;

//------------------------------------------------------------------------------
/*!
  \defgroup ZmqHelper Helper functions for ZMQ handling
*/

//-----------------------------------------------------------------------------
/*!
  \ingroup ZmqHelper
  \brief Build 2 part ZMQ message from 2 strings
  \param str1   string for 1st part
  \param str2   string for 2nd part
  \returns 2 part ZMQ message as vector of messages
*/

zmsgv_t String2Zmsg(const string& str1, const string& str2) {
  vector<zmq::message_t> res;
  res.push_back(String2Zmsg(str1));
  res.push_back(String2Zmsg(str2));
  return res;
}

//-----------------------------------------------------------------------------
/*!
  \ingroup ZmqHelper
  \brief Return short dump of ZMQ message as string
  \param zmsg   ZMQ message

  Returns a string of the form
```
s=nnnnnn: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 : xxxxxxxxxxxxxxxx
```
  starting with message size (`s=nnnnnn`), followed by up to 16 bytes first
  as 2 digit hex and second as ASIC characters.
*/

string ZmsgDump(const zmsg_t& zmsg) {
  ostringstream ss;
  static const size_t kMaxDump = 15;
  size_t msize = zmsg.size();
  size_t msize_cap = min(msize, kMaxDump);
  size_t msize_fil = kMaxDump - msize_cap;
  auto mbyte = zmsg.data<char>();
  ss << "s=" << setw(6) << msize << setw(0) << ":"
     << setfill('0') << hex;
  for (size_t i=0; i<msize_cap; i++) {
    unsigned int val = static_cast<unsigned char>(mbyte[i]);
    ss << " " << setw(2) << val << setw(0);
  }
  for (size_t i=0; i<msize_fil; i++) ss << "   ";
  ss << (msize > kMaxDump ? " .." : "   ");
  ss << " : ";
  for (size_t i=0; i<msize_cap; i++) {
    char c = mbyte[i];
    if (c < 32 || c >= 127) c = '.';
    ss << c;
  }
  if (msize > kMaxDump) ss << "..";
  return ss.str();
}

//-----------------------------------------------------------------------------
/*!
  \ingroup ZmqHelper
  \brief Return short dump of a multipart message
  \param zmsgv   ZMQ multipart message (as vector of message)

  Returns the number of parts and one line per message as returned by
  ZmsgDump(const zmsg_t&).
*/

string ZmsgDump(const zmsgv_t& zmsgv) {
  ostringstream ss;
  ss << "size: " << zmsgv.size();
  for (auto& zmsg: zmsgv) ss << "\n  " << ZmsgDump(zmsg);
  return ss.str();
}

} // end namespace Dca
