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

      void CYadomsUpdate::doWork(TaskProgressFunc progressCallback)
      {
         ::update::worker::CYadoms worker;

         //adapt the progress callback (signature are same; but could change in future, so an adaptation is better)
         ::update::worker::CYadoms::WorkerProgressFunc adapter = boost::bind(progressCallback, _1, _2, _3, _4, _5);
         worker.update(m_versionToUpdate, adapter);
      }

   } //namespace update
} //namespace task
