#pragma once
#include "shared/DataContainer.h"
#include "WorkerTools.h"

namespace update
{
   namespace worker
   {
      class IUpdateChecker
      {
      public:
         virtual ~IUpdateChecker(){}

         virtual void scanForUpdates(CWorkerTools::WorkerProgressFunc progressCallback) = 0;

         virtual shared::CDataContainer getUpdates(bool includePreleases) const = 0;
      };
   } // namespace worker
} // namespace update


