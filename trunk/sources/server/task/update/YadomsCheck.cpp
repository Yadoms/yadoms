#include "stdafx.h"
#include "YadomsCheck.h"
#include <shared/Log.h>
#include "update/worker/Yadoms.h"

namespace task {
   namespace update {

      std::string CYadomsCheck::m_taskName = "yadoms.checkForUpdate";

      CYadomsCheck::CYadomsCheck()
      {
      }

      CYadomsCheck::~CYadomsCheck()
      {
      }

      const std::string & CYadomsCheck::getName()
      {
         return m_taskName;
      }

      bool CYadomsCheck::doWork(TaskProgressFunc progressCallback)
      {
         try
         {
            ::update::worker::CYadoms worker;

            //adapt the progress callback (signature are same; but could change in future, so an adaptation is better)
            ::update::worker::CYadoms::WorkerProgressFunc adapter = boost::bind(progressCallback, _1, _2, _3);
            worker.checkForUpdate(adapter);
            return true;
         }
         catch (std::exception & e)
         {
            YADOMS_LOG(error) << "Error in yadoms check for update task " << e.what();
            progressCallback(false, 100.0f, e.what());
         }
         return false;
      }

   } //namespace update
} //namespace task
