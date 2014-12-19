#include "stdafx.h"
#include "Manager.h"
#include "database/IJobRequester.h"
#include "Job.h"
#include "condition/ConditionFactory.h"
#include "NotificationObserverForJobsManager.h"
#include <shared/exception/EmptyResult.hpp>

namespace job
{

CManager::CManager(boost::shared_ptr<database::IJobRequester> dbRequester, boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
   boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter, boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester)
   :m_pluginGateway(pluginGateway),
   m_dbRequester(dbRequester),
   m_conditionFactory(new condition::CConditionFactory(dbAcquisitionRequester)),
   m_notificationCenter(notificationCenter)
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
      if ((*it)->Enable)
         createJob(*it);
   }

   //TODO à supprimer, pour test
   //boost::shared_ptr<database::entities::CJob> jobData(new database::entities::CJob);
   //jobData->Id = 1;
   //jobData->Name = "testJob";
   //jobData->Description = "test";
   //jobData->Enable = true;
   //jobData->Triggers = std::string("{ \"and\" : [ { \"is\" : { \"keywordId\" : \"31\", \"expectedState\" : \"1\" } }, { \"is\" : { \"keywordId\" : \"32\", \"expectedState\" : \"0\" } } ] }");
   //jobData->Actions = std::string("{ \"actions\" : [ { \"type\" : \"command\", \"deviceId\" : \"6\", \"keywordId\" : \"14\", \"value\" : \"1\" } ] }");
   //createJob(jobData);
   //boost::shared_ptr<database::entities::CJob> jobData2(new database::entities::CJob);
   //jobData2->Id = 2;
   //jobData2->Name = "testJob2";
   //jobData2->Description = "test2";
   //jobData2->Enable = true;
   //jobData2->Triggers = std::string("{ \"and\" : [ { \"is\" : { \"keywordId\" : \"31\", \"expectedState\" : \"0\" } }, { \"is\" : { \"keywordId\" : \"32\", \"expectedState\" : \"1\" } } ] }");
   //jobData2->Actions = std::string("{ \"actions\" : [ { \"type\" : \"command\", \"deviceId\" : \"6\", \"keywordId\" : \"14\", \"value\" : \"0\" } ] }");
   //createJob(jobData2);
   //\TODO à supprimer, pour test

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

void CManager::createJob(boost::shared_ptr<database::entities::CJob> jobData)
{
   try
   {
      boost::shared_ptr<IJob> newJob(new CJob(jobData, m_notificationObserver, m_pluginGateway, *m_conditionFactory));
      m_jobs.push_back(newJob);
   }
   catch(shared::exception::CEmptyResult& e)
   {
      YADOMS_LOG(error) << "Invalid job \"" << jobData->Name() << "\" condition configuration, element not found in database : " << e.what();
   }
   catch(shared::exception::CInvalidParameter& e)
   {
      YADOMS_LOG(error) << "Invalid job \"" << jobData->Name() << "\" condition configuration, invalid parameter : " << e.what();
   }
   catch(shared::exception::COutOfRange& e)
   {
      YADOMS_LOG(error) << "Invalid job \"" << jobData->Name() << "\" condition configuration, out of range : " << e.what();
   }
}

} // namespace job	
	
	