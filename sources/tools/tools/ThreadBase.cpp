#include "stdafx.h"
#include "ThreadBase.h"
#include "Log.h"

const int CThreadBase::DefaultStopTimeoutSeconds = 10;


CThreadBase::CThreadBase(const std::string & threadName)
   :m_threadName(threadName), m_threadStatus(kStopped), m_stopTimeoutSeconds(DefaultStopTimeoutSeconds)
{
}

CThreadBase::~CThreadBase(void)
{
}

void CThreadBase::start()
{
   BOOST_ASSERT(getStatus() == kStopped);

   //start the thread
   changeStatus(kStarting);
   m_thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&CThreadBase::doWorkInternal, this)));
}

bool CThreadBase::stop()
{
   if (getStatus() == kStopped)
      return true;   // Already stopped

   // Request to stop and wait
   requestToStop();
   if (!waitForStop(m_stopTimeoutSeconds))
   {
      YADOMS_LOG(warning) << "Stopping thread " << getName() << " : timeout";
      return false;
   }

   m_thread.reset();
   return true;
}

void CThreadBase::requestToStop()
{
   changeStatus(kStopRequested);
   m_thread->interrupt();
}

bool CThreadBase::waitForStop(int seconds)
{
   BOOST_ASSERT(m_thread);

   bool stopped = false;

   if (seconds)
   {
      // A timeout was specified
      stopped = m_thread->timed_join(boost::posix_time::time_duration(0, 0, seconds, 0)); 
   }
   else
   {
      // no timeout specified
      m_thread->join();
      stopped = true;
   }

   if (stopped)
      changeStatus(kStopped);

   return stopped;
}

CThreadBase::EStatus CThreadBase::getStatus() const
{
   // TODO : rendre m_threadStatus thread-safe
   return m_threadStatus;
}

void CThreadBase::changeStatus(const EStatus & newStatus)
{
   m_threadStatus = newStatus;
   m_statusChanged(newStatus);
}

void CThreadBase::doWorkInternal()
{
   //manage the doWork method. It aims is to manage the thread state around doWork
   changeStatus(kRunning);
   try 
   {
      doWork();
   } 
   catch(boost::thread_interrupted&)
   {
      // Thread is stopped
   }

   changeStatus(kStopped);
}

void CThreadBase::registerStatusChanged(void *pFunc(const EStatus & newStatus), void * pInstance)
{
   //	m_statusChanged.connect( boost::bind( pFunc, pInstance, _1 ) );
}