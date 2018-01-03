#pragma once
#include "shared/DataContainer.h"

namespace update
{
   namespace worker
   {
      class IUpdateChecker
      {
      public:
         virtual ~IUpdateChecker(){}

         virtual void forceRebuildUpdates() = 0;

         virtual shared::CDataContainer getUpdates(bool includePreleases) const = 0;
      };
   } // namespace worker
} // namespace update


