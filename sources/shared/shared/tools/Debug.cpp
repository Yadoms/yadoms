#include "stdafx.h"
#include "Debug.h"
#include <Poco/Thread.h>
#include <shared/Log.h>

namespace shared {
   namespace tools {

      void CDebug::LogThreadId(const std::string & threadName)
      {
         Poco::Thread * currentThread = Poco::Thread::current();
         if (currentThread != NULL)
         {
            YADOMS_LOG(debug) << "Thread " << threadName << " tid=" << currentThread->tid() ;
         }         
         else
         {
            std::string threadId = boost::lexical_cast<std::string>(boost::this_thread::get_id());
            unsigned long threadNumber = 0;
            sscanf(threadId.c_str(), "%lx", &threadNumber);
            YADOMS_LOG(debug) << "Thread " << threadName << " tid=" << threadNumber;
         }
      }
      
   } // namespace tools 
} // namespace shared