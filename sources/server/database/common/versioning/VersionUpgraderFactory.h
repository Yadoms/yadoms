#pragma once

#include "IVersionUpgrade.h"


namespace database
{
   namespace common
   {
      namespace versioning
      {
         class CVersionUpgraderFactory
         {
         public:
            static boost::shared_ptr<IVersionUpgrade> GetUpgrader();

         private:
            CVersionUpgraderFactory() = delete;
            virtual ~CVersionUpgraderFactory() = delete;
         };
      } //namespace versioning
   } //namespace common
} //namespace database 


