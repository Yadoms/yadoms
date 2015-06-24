#include "stdafx.h"
#include "ThreadBase.h"
#include <shared/Thread.h>
#include "Log.h"

namespace shared
{

const int CThreadBase::DefaultStopTimeoutSeconds = 100;


CThreadBase::CThreadBase(const std::string & threadName, const bool bRethrowDoWorkExceptions)
   :m_threadName(threadName), m_stopTimeoutSeconds(DefaultStopTimeoutSeconds), m_bRethrowDoWorkExceptions(bRethrowDoWorkExceptions), m_stopping(false)
{
	BOOST_ASSERT(threadName != "");
}

CThreadBase::~CThreadBase()
{
   stop();
}

void CThreadBase::start()
{
   BOOST_ASSERT(!m_thread);

   //start the thread
   m_thread.reset(new boost::thread(boost::bind(&CThreadBase::doWorkInternal, this)));

   YADOMS_LOG(debug) << "Thread Id=" << m_thread->get_id() << " Name = " << getName();
}

void CThreadBase::stop()
{
   if (!m_thread)
      return;   // Already stopped

   YADOMS_LOG(information) << "Stopping thread " << getName();

   // Request to stop and wait
   requestToStop();
   if (waitForStop(m_stopTimeoutSeconds))
   {
      m_thread.reset();
      return;
   }

   // Gracefully stop fails, try to kill
   YADOMS_LOG(warning) << "Stopping thread " << getName() << " fails : timeout. Force kill.";
   CThread::killThread(m_thread->native_handle());
}

void CThreadBase::requestToStop()
{
   m_stopping = true;
   m_thread->interrupt();
}

bool CThreadBase::waitForStop(int seconds)
{
   BOOST_ASSERT(m_thread);

#ifndef _DEBUG // Timeout is a last-chance solution : it must not be used in DEBUG. Threads have to stop gracefully.
   if (seconds)
   {
      // A timeout was specified
      return m_thread->timed_join(boost::posix_time::time_duration(0, 0, seconds, 0)); 
   }
#endif

   // no timeout specified
   m_thread->join();
   m_stopping = false;
   return true;
}

bool CThreadBase::isStopping() const
{
   // m_thread->interruption_requested() is not enough, as it returns false as soon as boost::thread_interrupted was catched,
   // that can be do in user-code.
   return m_thread->interruption_requested() || m_stopping;
}

void CThreadBase::doWorkInternal()
{
   //manage the doWork method. It aims is to manage the thread state around doWork
   try 
   {
	  //configure the log with thread name
	  YADOMS_LOG_CONFIGURE(getName());

      doWork();
   } 
   catch(boost::thread_interrupted&)
   {
      // Thread is stopped      
   }
   catch(std::exception &)
   {
      if(m_bRethrowDoWorkExceptions)
         throw;
   }
   catch(...)
   {
      //this exception may occur when bad states are reached
      //do not do anything here
      //do not catch std::exception& because most of time it is not valid
      if(m_bRethrowDoWorkExceptions)
         throw;
   }
}

} // namespace shared
