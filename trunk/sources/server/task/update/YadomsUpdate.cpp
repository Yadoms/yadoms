#include "stdafx.h"
#include "YadomsUpdate.h"
#include <shared/Log.h>
#include "update/worker/Yadoms.h"

namespace task {
   namespace update {

      std::string CYadomsUpdate::m_taskName = "yadoms.update";

      CYadomsUpdate::CYadomsUpdate(const shared::CDataContainer & versionToUpdate)
         :m_versionToUpdate(versionToUpdate)
      {
      }

      CYadomsUpdate::~CYadomsUpdate()
      {
      }

      const std::string & CYadomsUpdate::getName()
      {
         return m_taskName;
      }

      bool CYadomsUpdate::doWork(TaskProgressFunc progressCallback)
      {
         try
         {
            ::update::worker::CYadoms worker;

            //adapt the progress callback (signature are same; but could change in future, so an adaptation is better)
            ::update::worker::CYadoms::WorkerProgressFunc adapter = boost::bind(progressCallback, _1, _2, _3);

            worker.update(m_versionToUpdate, adapter);
            return true;
         }
         catch (std::exception & e)
         {
            YADOMS_LOG(error) << "Error in yadoms update task " << e.what();
            progressCallback(false, 100.0f, e.what());
         }
         return false;
      }

   } //namespace update
} //namespace task
