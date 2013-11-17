#include "ThreadBase.h"
#include "boost/bind.hpp"

CThreadBase::CThreadBase()
	:m_threadName(""), m_threadStatus(kStopped)
{
}

CThreadBase::CThreadBase(const std::string & threadName)
	:m_threadName(threadName), m_threadStatus(kStopped)
{
}

CThreadBase::~CThreadBase(void)
{
}

void CThreadBase::start()
{
	//start the thread
	m_thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&CThreadBase::doWorkInternal, this)));
}

void CThreadBase::stop()
{
	//request to stop and wait
	requestToStop();
	while(getStatus() != kStopped)
	{
		boost::this_thread::sleep(boost::posix_time::millisec(10));
	}
	/*
	// Stop the thread
    m_thread->interrupt();
    m_thread->join();
    //m_thread->start_thread
    //m_thread->start_thread
    //m_pThread = NULL;

    // Call plugin stop method to free ressources
    //m_pPlugin->stop();
	*/
}

void CThreadBase::requestToStop()
{
	changeStatus(kStopRequested);
}

const CThreadBase::EStatus CThreadBase::getStatus()
{
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

void CThreadBase::setName(const std::string & name)
{
	m_threadName = name;
}

const std::string CThreadBase::getName()
{
	return m_threadName;
}

void CThreadBase::registerStatusChanged(void *pFunc(const EStatus & newStatus), void * pInstance)
{
//	m_statusChanged.connect( boost::bind( pFunc, pInstance, _1 ) );
}