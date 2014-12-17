#include "stdafx.h"
#include "Manager.h"
#include "database/IJobRequester.h"
#include "Job.h"
#include "condition/ConditionFactory.h"
#include "NotificationObserverForJobsManager.h"

namespace job
{

CManager::CManager(boost::shared_ptr<database::IJobRequester> dbRequester, boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
   boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter)
   :m_pluginGateway(pluginGateway), m_dbRequester(dbRequester), m_conditionFactory(new condition::CConditionFactory), m_notificationCenter(notificationCenter)
{        
}

CManager::~CManager()
{        
}

void CManager::start()
{
   BOOST_ASSERT_MSG(m_jobs.empty(), "Some job are already started, are you sure that manager was successfuly stopped ?");

   // Register notification observer
   m_notificationObserver.reset(new CNotificationObserverForJobsManager(m_notificationCenter));

   // Create all jobs
   std::vector<boost::shared_ptr<database::entities::CJob> > jobs = m_dbRequester->getJobs();
   for (std::vector<boost::shared_ptr<database::entities::CJob> >::const_iterator it = jobs.begin(); it != jobs.end(); ++it)
   {
      boost::shared_ptr<IJob> newJob(new CJob(*it, m_notificationObserver, m_pluginGateway, *m_conditionFactory));
      m_jobs.push_back(newJob);
   }

   // Start all jobs
   for (std::vector<boost::shared_ptr<IJob> >::const_iterator it = m_jobs.begin(); it != m_jobs.end(); ++it)
   {
      (*it)->start();
   }
}

void CManager::stop()
{
   // Stop all jobs
   for (std::vector<boost::shared_ptr<IJob> >::const_iterator it = m_jobs.begin(); it != m_jobs.end(); ++it)
   {
      (*it)->start();
   }

   // Free all jobs
   m_jobs.clear();

   // Unregister notification observer
   m_notificationObserver.reset();
}

} // namespace job	
	
	