#include "stdafx.h"
#include "JobThread.h"

namespace job
{

CJobThread::CJobThread(const std::string & jobName, IJobThreadHandler& handler)
   :CThreadBase("job #" + jobName), m_handler(handler)
{
}

CJobThread::~CJobThread()
{         
}

void CJobThread::doWork()
{
   m_handler.doWork();
}


} // namespace job	
	
	