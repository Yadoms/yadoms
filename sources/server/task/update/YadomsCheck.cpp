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

      void CYadomsCheck::doWork(TaskProgressFunc progressCallback)
      {
         ::update::worker::CYadoms worker;
         worker.checkForUpdate(boost::bind(progressCallback, _1, _2, _3, _4, _5));
      }

   } //namespace update
} //namespace task
