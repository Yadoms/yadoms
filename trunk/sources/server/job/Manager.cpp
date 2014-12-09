#include "stdafx.h"
#include "Manager.h"
#include "database/IJobRequester.h"
#include "Job.h"

namespace job
{

CManager::CManager(boost::shared_ptr<database::IJobRequester> dbRequester, boost::shared_ptr<communication::ISendMessageAsync> pluginGateway)
   :m_pluginGateway(pluginGateway), m_dbRequester(dbRequester)
{        
}

CManager::~CManager()
{        
}

void CManager::start()
{
   BOOST_ASSERT_MSG(m_jobs.empty(), "Some job are already started, are you sure that manager was successfuly stopped ?");

   // Create all jobs
   std::vector<boost::shared_ptr<database::entities::CJob> > jobs = m_dbRequester->getJobs();
   for (std::vector<boost::shared_ptr<database::entities::CJob> >::const_iterator it = jobs.begin(); it != jobs.end(); ++it)
   {
      boost::shared_ptr<database::entities::CJob> jobData = *it;
      boost::shared_ptr<IJob> newJob(new CJob(jobData->Id(), jobData->Name(), jobData->Triggers(), jobData->Actions(), m_pluginGateway));
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
}

} // namespace job	
	
	