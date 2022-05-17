// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#ifndef included_Dca_LoggerMessage
#define included_Dca_LoggerMessage 1

#include "ChronoDefs.hpp"

#include <string>

namespace cbm {

/*! \struct LoggerMessage
  \brief Holds the context of a Logger message
*/

struct LoggerMessage {
                    LoggerMessage() = default;
                    LoggerMessage(const sctime_point& time, int sev,
                                  string&& tname, string&& keys, string&& text) :
                      fTime(time),
                      fSevId(sev),
                      fThreadName(move(tname)),
                      fKeys(move(keys)),
                      fMessage(move(text)) {}
                    LoggerMessage(const LoggerMessage& rhs) = delete;
                    LoggerMessage(LoggerMessage&& rhs) = default;

  sctime_point      fTime {};               //!< timestamp
  int               fSevId {0};             //!< severity
  string            fThreadName {""};       //!< thread name
  string            fKeys {""};             //!< key set
  string            fMessage {""};          //!< message body
};

} // end namespace cbm

#endif
