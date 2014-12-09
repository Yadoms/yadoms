#include "stdafx.h"
#include "Job.h"
#include "Condition.h"
#include "ActionList.h"

namespace job
{

CJob::CJob(int id, const std::string& name, const shared::CDataContainer& triggers, const shared::CDataContainer& actions, boost::shared_ptr<communication::ISendMessageAsync> pluginGateway)
   :m_name(name), m_id(id), m_condition(new CCondition(triggers)), m_actions(new CActionList(actions, pluginGateway))
{
}

CJob::~CJob()
{         
}

void CJob::start()
{
   m_thread.reset(new CJobThread(m_name, *this));
   m_thread->start();
}

void CJob::stop()
{
   m_thread->stop();
   m_thread.reset();
}

void CJob::doWork()
{
   while (true)
   {
      m_condition->wait();
      m_actions->doAll();
   }
}

} // namespace job	
	
	