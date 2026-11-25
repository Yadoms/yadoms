#pragma once

#include "IVersionUpgrade.h"


namespace database::common::versioning
{
   class CVersionUpgraderFactory
   {
   public:
      static boost::shared_ptr<IVersionUpgrade> GetUpgrader();

   private:
      CVersionUpgraderFactory() = default;
      virtual ~CVersionUpgraderFactory() = default;
   };
}
