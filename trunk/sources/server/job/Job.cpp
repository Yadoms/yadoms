#include "stdafx.h"
#include "Job.h"
#include "condition/ConditionRoot.h"
#include "ActionList.h"
#include "INotificationObserverForJobsManager.h"

namespace job
{

CJob::CJob(boost::shared_ptr<database::entities::CJob> jobData,
   boost::shared_ptr<INotificationObserverForJobsManager> notificationObserver,
   boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
   condition::IConditionFactory& conditionFactory)
   :m_name(jobData->Name()),
   m_id(jobData->Id()),
   m_condition(new condition::CConditionRoot(jobData->Triggers(), conditionFactory)),
   m_actions(new CActionList(jobData->Actions(), pluginGateway)),
   m_notificationObserver(notificationObserver)
{
   m_condition->registerToNotificationCenter(m_notificationObserver);
}

CJob::~CJob()
{         
   m_condition->unregisterFromNotificationCenter(m_notificationObserver);
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
	
	