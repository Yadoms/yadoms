#include "stdafx.h"
#include "Yadoms.h"
#include <shared/Log.h>
#include "update/worker/Yadoms.h"

namespace task {
   namespace update {

      std::string CYadoms::m_taskName = "system.yadomsUpdate";

      CYadoms::CYadoms(boost::shared_ptr<::update::source::CYadoms> updateSource, bool onlyCheckForUpdate)
         :m_updateSource(updateSource), m_onlyCheckForUpdate(onlyCheckForUpdate)
      {
      }

      CYadoms::~CYadoms()
      {
      }

      const std::string & CYadoms::getName()
      {
         return m_taskName;
      }

      bool CYadoms::doWork(TaskProgressFunc progressCallback)
      {
         try
         {
            ::update::worker::CYadoms worker(m_updateSource);

            //adapt the progress callback (signature are same; but could change in future, so an adaptation is better)
            ::update::worker::IWorker::WorkerProgressFunc adapter = boost::bind(progressCallback, _1, _2, _3);

            if (m_onlyCheckForUpdate)
               worker.checkForUpdateAsync(adapter);
            else
               worker.updateAsync(adapter);
            
            return true;
         }
         catch (std::exception & e)
         {
            if (m_onlyCheckForUpdate)
               YADOMS_LOG(error) << "Error in yadoms check for update task " << e.what();
            else
               YADOMS_LOG(error) << "Error in yadoms update task " << e.what();

            progressCallback(false, 100.0f, e.what());
         }
         return false;
      }

   } //namespace update
} //namespace task
