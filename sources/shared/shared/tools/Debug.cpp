#include "stdafx.h"
#include "Debug.h"
#include <Poco/Thread.h>
#include <shared/Log.h>
#include <shared/tools/ThreadMonitoring.h>

namespace shared {
   namespace tools {

      static shared::tools::CThreadMonitoring monitor;
      static Poco::Thread monitoringThread("CThreadMonitoring");

      void CDebug::CreateThreadMonitoring()
      {
         monitoringThread.start(monitor);
      }

      void CDebug::LogThreadId(const std::string & threadName)
      {
         unsigned long threadNumber = 0;

         Poco::Thread * currentThread = Poco::Thread::current();
         if (currentThread != NULL)
         {
            YADOMS_LOG(debug) << "Thread " << threadName << " tid=" << currentThread->tid() ;
            threadNumber = currentThread->tid();
         }         
         else
         {
            std::string threadId = boost::lexical_cast<std::string>(boost::this_thread::get_id());
            sscanf(threadId.c_str(), "%lx", &threadNumber);
            YADOMS_LOG(debug) << "Thread " << threadName << " tid=" << threadNumber;
         }

         monitor.addThreadToMonitor(threadNumber);
      }
      
   } // namespace tools 
} // namespace shared