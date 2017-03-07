#pragma once

#include <shared/Export.h>
#include "logInternal/ILogStream.h"
#include <Poco/Logger.h>
#include <Poco/Thread.h>

namespace shared
{
 
   //--------------------------------------------------------------
   /// \brief	    Log a message (stream way)
   //--------------------------------------------------------------
   #define YADOMS_LOG(lvl) shared::CLog::logStream()->lvl()

   //--------------------------------------------------------------
   /// \brief	    Configure the logger by settings the current thread name
   //--------------------------------------------------------------
   #define YADOMS_LOG_CONFIGURE(threadname) shared::CLog::setThreadName(threadname);
   

   //--------------------------------------------------------------
   /// \brief	    Class which keeps statically the logger
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CLog
   {
   public:
      //--------------------------------------------------------------
      /// \brief	    Get the main logger
      /// \return     The main logger
      //--------------------------------------------------------------
      static Poco::Logger& logger();

      //--------------------------------------------------------------
      /// \brief	    Get the main logger stream
      /// \return     The main logger stream (shread ptr)
      //--------------------------------------------------------------
      static boost::shared_ptr<logInternal::ILogStream> logStream();

      //--------------------------------------------------------------
      /// \brief	    Set the thread name
      /// \param [in] name    Name of the thread
      //--------------------------------------------------------------
      static void setThreadName(const std::string & name);

      //--------------------------------------------------------------
      /// \brief	    Get the current thread name
      /// \return     Name of the thread (empty if never defined)
      //--------------------------------------------------------------
      static const std::string & getCurrentThreadName();

   private:
      //--------------------------------------------------------------
      /// \brief	    The thread names
      //--------------------------------------------------------------
      static std::map<Poco::Thread::TID, std::string> m_threadNames;
   };

} // namespace shared