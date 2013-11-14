#include "ThreadBase.h"
#include "boost/bind.hpp"

CThreadBase::CThreadBase(const std::string & threadName)
	:m_threadName(threadName), m_threadStatus(kStopped)
{
}


CThreadBase::~CThreadBase(void)
{
}


void CThreadBase::start()
{
	m_thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&CThreadBase::doWorkInternal, this)));
}

void CThreadBase::stop()
{
	requestToStop();
	while(getStatus() != kStopped)
	{
		boost::this_thread::sleep(boost::posix_time::millisec(10));
	}
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
	changeStatus(kRunning);
	doWork();
	changeStatus(kStopped);
}


const std::string CThreadBase::getName()
{
	return m_threadName;
}

void CThreadBase::registerStatusChanged(void *pFunc(const EStatus & newStatus), void * pInstance)
{
//	m_statusChanged.connect( boost::bind( pFunc, pInstance, _1 ) );
}