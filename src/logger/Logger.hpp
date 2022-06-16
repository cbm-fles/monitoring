// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020-2022 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#ifndef included_Cbm_Logger
#define included_Cbm_Logger 1

#include "FileDescriptor.hpp"
#include "LoggerMessage.hpp"
#include "LoggerSink.hpp"
#include "LoggerStream.hpp"

#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

namespace cbm {

// Note: the \file is required by doxygen to enable documentation of macros
/*!
  \file Logger.hpp
  \brief Logger header
*/

struct LoggerMessage; // forward declaration
class LoggerSink;     // forward declaration

class Logger {
public:
  Logger();
  virtual ~Logger();

  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;

  LoggerStream MakeStream(int sev,
                          const string& keys1,
                          const string& mid,
                          const string& keys2);

  string SeverityCode2Text(int sevcode, bool nothrow = false);
  int SeverityText2Code(const string& sevtext, bool nothrow = false);

  void OpenSink(const string& sname, int lvl);
  void CloseSink(const string& sname);
  vector<string> SinkList();
  int SinkLogLevel(const string& sname);
  void SetSinkLogLevel(const string& sname, int lvl);

  void QueueMessage(LoggerMessage&& msg);
  const string& HostName() const;
  const string& ProgName() const;

  static Logger& Ref();
  static Logger* Ptr();

  // some constants (!! when changed update definition of fSevCode2Text !!)
  static const int kELoopTimeout = 100; //!< logger flush time in ms
  enum LoggerSeverityLevel {
    kLogTrace = 0, //!< Trace (very verbose)
    kLogDebug,     //!< Debug (verbose)
    kLogInfo,      //!< Info
    kLogNote,      //!< Note  (important notification)
    kLogWarning,   //!< Warning
    kLogError,     //!< Error
    kLogFatal      //!< Fatal  (no recovery possible)
  };

private:
  void Stop();
  void Wakeup();
  void EventLoop();
  LoggerSink& SinkRef(const string& sname);

private:
  using msgvec_t = vector<LoggerMessage>;
  using sink_uptr_t = unique_ptr<LoggerSink>;
  using smap_t = unordered_map<string, sink_uptr_t>;

  FileDescriptor fEvtFd{};      //!< fd for eventfd file
  thread fThread{};             //!< worker thread
  msgvec_t fMsgVec{};           //!< message list
  mutex fMsgVecMutex{};         //!< mutex for fMsgVec access
  string fHostName{""};         //!< hostname
  string fProgName{""};         //!< program name
  bool fStopped{false};         //!< signals thread rundown
  vector<string> fSevCode2Text; //!< code to text map (init in ctor)
  smap_t fSinkMap{};            //!< sink registry
  mutex fSinkMapMutex{};        //!< mutex for fSinkMap access
  static Logger* fpSingleton;   //!< \glos{singleton} this
};

} // end namespace cbm

// framework macros, usually not used directly
#define CBMLOG(sel, sev, keys1, mid, keys2)                                    \
  if (sel)                                                                     \
  ::cbm::Logger::Ref().MakeStream(sev, keys1, mid, keys2).Stream()
#define CBMLOGGEN(sev, mid, keys)                                              \
  CBMLOG(sev >= LogLevel(), sev, LogKeys(), mid, keys)
#define CBMLOGGEN1(sev, mid, keys) CBMLOG(true, sev, LogKeys(), mid, keys)
// for looging when LogLevel() and LogKeys() available, e.g. DClass'es
#define CBMLOGFAT(mid, keys) CBMLOGGEN1(::cbm::Logger::kLogFatal, mid, keys)
#define CBMLOGERR(mid, keys) CBMLOGGEN(::cbm::Logger::kLogError, mid, keys)
#define CBMLOGWAR(mid, keys) CBMLOGGEN(::cbm::Logger::kLogWarning, mid, keys)
#define CBMLOGNOT(mid, keys) CBMLOGGEN(::cbm::Logger::kLogNote, mid, keys)
#define CBMLOGINF(mid, keys) CBMLOGGEN(::cbm::Logger::kLogInfo, mid, keys)
#define CBMLOGDEB(mid, keys) CBMLOGGEN(::cbm::Logger::kLogDebug, mid, keys)
#define CBMLOGTRA(mid, keys) CBMLOGGEN(::cbm::Logger::kLogTrace, mid, keys)
#define CBMLOGTRAOBJ(obj, mid, keys)                                           \
  CBMLOG(::cbm::Logger::kLogTrace >= (obj).LogLevel(),                         \
         ::cbm::Logger::kLogTrace, (obj).LogKeys(), mid, keys)
// for looging unconditionally when LogKeys() availabke
#define CBMLOGERRI(mid, keys) CBMLOGGEN1(::cbm::Logger::kLogError, mid, keys)
#define CBMLOGNOTI(mid, keys) CBMLOGGEN1(::cbm::Logger::kLogNote, mid, keys)
// for looging unconditionally without LogKeys()
#define CBMLOGFAT1(keys1, mid)                                                 \
  CBMLOG(true, ::cbm::Logger::kLogFatal, keys1, mid, "")
#define CBMLOGERR1(keys1, mid)                                                 \
  CBMLOG(true, ::cbm::Logger::kLogError, keys1, mid, "")
#define CBMLOGNOT1(keys1, mid)                                                 \
  CBMLOG(true, ::cbm::Logger::kLogNote, keys1, mid, "")

#include "Logger.ipp"

#endif
