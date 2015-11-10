#include "stdafx.h"
#include "ThreadMonitoring.h"
#include <Poco/Thread.h>
#include <shared/Log.h>

namespace shared { namespace tools {

   CThreadMonitoring::CThreadMonitoring()
   {
   }

   CThreadMonitoring::~CThreadMonitoring()
   {
   }

   void CThreadMonitoring::addThreadToMonitor(int threadId)
   {
      for (std::vector<int>::iterator i = m_threadIdToMonitor.begin(); i != m_threadIdToMonitor.end(); ++i)
      {
         if (*i == threadId)
            return;
      }
      m_threadIdToMonitor.push_back(threadId);
   }
   
   void CThreadMonitoring::run()
   {

   }

}} // namespace shared::tools