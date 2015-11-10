#include "stdafx.h"
#include "ThreadMonitoring.h"
#include <Poco/Thread.h>
#include <Windows.h>
#include <shared/Log.h>
#include "Processthreadsapi.h"

namespace shared { namespace tools {

   CThreadMonitoring::CThreadMonitoring()
   {
   }

   CThreadMonitoring::~CThreadMonitoring()
   {
   }

   // Returns true if thread times could be queried and its results are usable,
   // false otherwise. Error handling is minimal, considering throwing detailed
   // exceptions instead of returning a simple boolean.
   bool get_remote_thread_times(DWORD thread_id, FILETIME & kernel_time, FILETIME & user_time)
   {
      FILETIME creation_time = { 0 };
      FILETIME exit_time = { 0 };
      HANDLE thread_handle = OpenThread(THREAD_QUERY_INFORMATION, FALSE, thread_id);
      if (thread_handle == NULL) 
         return false;

      bool success = GetThreadTimes(thread_handle, &creation_time, &exit_time, &kernel_time, &user_time) != 0;

      CloseHandle(thread_handle);
      return success;
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
      Poco::Thread::current()->sleep(1000 * 10);

      while (true)
      {
         for (std::vector<int>::iterator i = m_threadIdToMonitor.begin(); i != m_threadIdToMonitor.end(); ++i)
         {
            FILETIME userSpaceTime = { 0 };
            FILETIME kernelSpaceTime = { 0 };

            if (get_remote_thread_times(*i, kernelSpaceTime, userSpaceTime))
            {
               YADOMS_LOG(debug) << "Monitoring thread @" << (*i) << " UserSpace : Low " << userSpaceTime.dwLowDateTime << " High " << userSpaceTime.dwHighDateTime;
               YADOMS_LOG(debug) << "Monitoring thread @" << (*i) << " KernelSpace : Low " << kernelSpaceTime.dwLowDateTime << " High " << kernelSpaceTime.dwHighDateTime;
            }
         }


         //sleep 60 secondes
         Poco::Thread::current()->sleep(1000 * 60);

      }
   }
   

}} // namespace shared::tools