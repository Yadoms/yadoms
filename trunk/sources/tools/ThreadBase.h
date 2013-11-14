#pragma once

#include <string>
#include "boost/thread.hpp"
#include <boost/signals2.hpp>

class CThreadBase
{
public:
	CThreadBase(const std::string & threadName);
	virtual ~CThreadBase(void);

	enum EStatus
	{
		kRunning,
		kStopRequested,
		kStopped
	};

public:
	void start();
	void requestToStop();
	void stop();

	const EStatus getStatus();
	const std::string getName();

	
	void registerStatusChanged(void *pFunc(const EStatus & newStatus), void * pInstance);

protected:
	virtual void doWork() =0;

private:
	void changeStatus(const EStatus & newStatus);
	void doWorkInternal();

private:
	std::string m_threadName;
	boost::shared_ptr<boost::thread> m_thread;
	EStatus m_threadStatus;
	boost::signals2::signal<void(const EStatus & newStatus)> m_statusChanged;
};

