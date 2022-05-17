// SPDX-License-Identifier: GPL-3.0-only
// (C) Copyright 2020-2022 GSI Helmholtzzentrum für Schwerionenforschung
// Original author: Walter F.J. Mueller <w.f.j.mueller@gsi.de>

#ifndef included_Dca_Logger
#define included_Dca_Logger 1

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

struct LoggerMessage;                       // forward declaration
class LoggerSink;                           // forward declaration

class Logger {
  public:
                    Logger();
  virtual           ~Logger();
  
                    Logger(const Logger&) = delete;
                    Logger& operator=(const Logger&) = delete;

  LoggerStream      MakeStream(int sev, const string& keys1,
                               const string& mid, const string& keys2);

  string            SeverityCode2Text(int sevcode, bool nothrow=false);
  int               SeverityText2Code(const string& sevtext, bool nothrow=false);

  void              OpenSink(const string& sname, int lvl);
  void              CloseSink(const string& sname);
  vector<string>    SinkList();
  int               SinkLogLevel(const string& sname);
  void              SetSinkLogLevel(const string& sname, int lvl);

  void              QueueMessage(LoggerMessage&& msg);
  const string&     HostName() const;
  const string&     ProgName() const;

  static Logger&    Ref();
  static Logger*    Ptr();

  // some constants (!! when changed update definition of fSevCode2Text !!)
  static const int  kELoopTimeout = 100;    //!< logger flush time in ms
  enum LoggerSeverityLevel {
    kLogTrace = 0,                          //!< Trace (very verbose)
    kLogDebug,                              //!< Debug (verbose)
    kLogInfo,                               //!< Info
    kLogNote,                               //!< Note  (important notification)
    kLogWarning,                            //!< Warning
    kLogError,                              //!< Error
    kLogFatal                               //!< Fatal  (no recovery possible)
  };

  private:
  void              Stop();
  void              Wakeup();
  void              EventLoop();
  LoggerSink&       SinkRef(const string& sname);

  private:
  using msgvec_t = vector<LoggerMessage>;
  using sink_uptr_t  = unique_ptr<LoggerSink>;
  using smap_t = unordered_map<string, sink_uptr_t>;

  FileDescriptor    fEvtFd {};              //!< fd for eventfd file
  thread            fThread {};             //!< worker thread
  msgvec_t          fMsgVec {};             //!< message list
  mutex             fMsgVecMutex {};        //!< mutex for fMsgVec access
  string            fHostName {""};         //!< hostname
  string            fProgName {""};         //!< program name
  bool              fStopped {false};       //!< signals thread rundown
  vector<string>    fSevCode2Text;          //!< code to text map (init in ctor)
  smap_t            fSinkMap {};            //!< sink registry
  mutex             fSinkMapMutex {};       //!< mutex for fSinkMap access
  static Logger*    fpSingleton;            //!< \glos{singleton} this
};

} // end namespace cbm

// framework macros, usually not used directly
#define DCALOG(sel,sev,keys1,mid,keys2) \
  if (sel) Logger::Ref().MakeStream(sev, keys1, mid, keys2).Stream()
#define DCALOGGEN(sev,mid,keys) DCALOG(sev>=LogLevel(), sev, LogKeys(),mid,keys)
#define DCALOGGEN1(sev,mid,keys) DCALOG(true, sev, LogKeys(),mid,keys)
// for looging when LogLevel() and LogKeys() available, e.g. DClass'es
#define DCALOGFAT(mid,keys) DCALOGGEN1(Logger::kLogFatal, mid, keys)
#define DCALOGERR(mid,keys) DCALOGGEN(Logger::kLogError, mid, keys)
#define DCALOGWAR(mid,keys) DCALOGGEN(Logger::kLogWarning, mid, keys)
#define DCALOGNOT(mid,keys) DCALOGGEN(Logger::kLogNote, mid, keys)
#define DCALOGINF(mid,keys) DCALOGGEN(Logger::kLogInfo, mid, keys)
#define DCALOGDEB(mid,keys) DCALOGGEN(Logger::kLogDebug, mid, keys)
#define DCALOGTRA(mid,keys) DCALOGGEN(Logger::kLogTrace, mid, keys)
#define DCALOGTRAOBJ(obj,mid,keys) \
  DCALOG(Logger::kLogTrace>=(obj).LogLevel(), Logger::kLogTrace, \
         (obj).LogKeys(), mid, keys)
// for looging unconditionally when LogKeys() availabke 
#define DCALOGERRI(mid,keys) DCALOGGEN1(Logger::kLogError, mid, keys)
#define DCALOGNOTI(mid,keys) DCALOGGEN1(Logger::kLogNote, mid, keys)
// for looging unconditionally without LogKeys() 
#define DCALOGFAT1(keys1,mid) DCALOG(true, Logger::kLogFatal, keys1, mid, "")
#define DCALOGERR1(keys1,mid) DCALOG(true, Logger::kLogError, keys1, mid, "")
#define DCALOGNOT1(keys1,mid) DCALOG(true, Logger::kLogNote, keys1, mid, "")

#include "Logger.ipp"

#endif
