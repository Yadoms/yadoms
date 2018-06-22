#include "stdafx.h"

#include "PurgeTask.h"
#include <shared/currentTime/Provider.h>
#include <shared/ServiceLocator.h>
#include <shared/Log.h>
#include "startupOptions/IStartupOptions.h"

namespace database
{
   namespace common
   {
      CPurgeTask::CPurgeTask(boost::shared_ptr<IAcquisitionRequester> acquisitionRequester,
                             boost::shared_ptr<IDatabaseRequester> sqlRequester)
         : m_acquisitionRequester(acquisitionRequester), m_sqlRequester(sqlRequester)
      {
         //retreive startup options
         const auto startupOptions = shared::CServiceLocator::instance().get<const startupOptions::IStartupOptions>();

         m_acquisitionLifetimeDays = startupOptions->getDatabaseAcquisitionLifetime();
      }

      CPurgeTask::~CPurgeTask()
      {
      }

      void CPurgeTask::run()
      {
         try
         {
            if (m_acquisitionLifetimeDays > 0)
            {
               YADOMS_LOG_CONFIGURE("Database purge task");

               //determine minimum datetime
               const boost::posix_time::ptime now(shared::currentTime::Provider().now().date());
               const boost::posix_time::time_duration realDuration = boost::posix_time::hours(24 * m_acquisitionLifetimeDays);
               const auto purgeDate = now - realDuration;

               YADOMS_LOG(information) << "Start purging database : removing acquisition of more than " << m_acquisitionLifetimeDays <<
                  " days : prior to " << purgeDate;

               // We have to purge by steps, to not lock the database too much time so other requests can fail
               const auto limitByStep = 10000;
               while (true)
               {
                  YADOMS_LOG(trace) << "Do a purge step...";
                  const auto count = m_acquisitionRequester->purgeAcquisitions(purgeDate,
                                                                               limitByStep);

                  if (count == 0)
                  {
                     // No more data to purge
                     YADOMS_LOG(information) << "End of purging database";
                     return;
                  }

                  //Don't do vacuum (take too much ressources)

                  boost::this_thread::sleep(boost::posix_time::millisec(100));
               }
            }
         }
         catch (std::exception& ex)
         {
            YADOMS_LOG(error) << "Error in purging database :" << ex.what();
         }
      }
   } //namespace common
} //namespace database 
