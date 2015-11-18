#include "stdafx.h"

#include "SQLitePurgeTask.h"
#include <shared/currentTime/Provider.h>
#include <shared/ServiceLocator.h>
#include "startupOptions/IStartupOptions.h"

namespace database { 
namespace sqlite { 


   CSQLitePurgeTask::CSQLitePurgeTask(boost::shared_ptr<IAcquisitionRequester> acquisitionRequester, boost::shared_ptr<CSQLiteRequester> sqlRequester)
      :m_acquisitionRequester(acquisitionRequester), m_sqlRequester(sqlRequester)
   {
      //retreive startup options
      boost::shared_ptr<startupOptions::IStartupOptions> startupOptions = shared::CServiceLocator::instance().get<startupOptions::IStartupOptions>();

      m_acquisitionLifetimeDays = startupOptions->getDatabaseAcquisitionLifetime();
   }

   CSQLitePurgeTask::~CSQLitePurgeTask()
   {
   }

   void CSQLitePurgeTask::run()
   {
      try
      {
         if (m_acquisitionLifetimeDays > 0)
         {
            YADOMS_LOG_CONFIGURE("Database purge task");

            //determine minimum datetime
            boost::posix_time::ptime now(shared::currentTime::Provider::now().date());
            boost::posix_time::time_duration realDuration = boost::posix_time::hours(24 * m_acquisitionLifetimeDays);
            boost::posix_time::ptime purgeDate = now - realDuration;

            YADOMS_LOG(information) << "Purging database : removing acquisition of more than " << m_acquisitionLifetimeDays << " days : prior to " << purgeDate;
            int count = m_acquisitionRequester->purgeAcquisitions(purgeDate);

            //if any data have been deleted, then call vacuum to free disk space
            if (count > 0)
               m_sqlRequester->vacuum();
         }
      }
      catch (std::exception & ex)
      {
         YADOMS_LOG(error) << "Error in purging database :" << ex.what();
      }
   }



} //namespace sqlite
} //namespace database 

