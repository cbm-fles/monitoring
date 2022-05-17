// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020-2021 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#include "LoggerStream.hpp"

#include "ChronoHelper.hpp"
#include "Logger.hpp"
#include "LoggerMessage.hpp"
#include "PThreadHelper.hpp"

namespace Dca {

/*! \class LoggerStream
  \brief Provides an `ostream` interface to which a message can be written

  The constructor will setup a message context with timestamp, severity and
  key set. The message body will be added via `Stream()` and `operator<<()`.
  The dectructor will finally queue the message to the Logger core.
 */

//-----------------------------------------------------------------------------
/*! \brief Constructor
  \param logger  reference to Logger
  \param sev     message severity
  \param keys1   primary environment keys
  \param mid     message id
  \param keys2   seconday environment keys

  Sets up a message context with
  - timestamp
  - severity from `sev`
  - key set composed from `keys1`, `mid`, and `keys2`
 */

LoggerStream::LoggerStream(Logger& logger, int sev, const string& keys1,
                           const string& mid, const string& keys2) :
  fLogger(logger),
  fTime(ScNow()),
  fSevId(sev),
  fThreadName(PThreadName()),
  fKeys(keys1),
  fSStream()
{
  if (mid.length())   fKeys += ",mid=" + mid;
  if (keys2.length()) {
    string_view keys2sv(keys2);             // view with dropped trailing ','
    if (keys2sv[keys2sv.length()-1] == ',') keys2sv.remove_suffix(1);
    fKeys += ",";
    fKeys += keys2sv;
  }
}

//-----------------------------------------------------------------------------
/*! \brief Destructor

  Queues the message for processing by the Logger core.
 */

LoggerStream::~LoggerStream()
{
  string text = fSStream.str();
  fLogger.QueueMessage(LoggerMessage{fTime, fSevId, move(fThreadName),
        move(fKeys), move(text)});
}
  
} // end namespace Dca
