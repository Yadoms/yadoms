#include "stdafx.h"
#include "VersionUpgraderFactory.h"
#include "Version_4_2_0.h"

namespace database
{
   namespace common
   {
      namespace versioning
      {
         CVersionUpgraderFactory::CVersionUpgraderFactory()
         {
         }

         CVersionUpgraderFactory::~CVersionUpgraderFactory()
         {
         }

         boost::shared_ptr<IVersionUpgrade> CVersionUpgraderFactory::GetUpgrader()
         {
            //change this line when a new database version is released
            return boost::make_shared<CVersion_4_2_0>();
         }
      } //namespace versioning
   } //namespace common
} //namespace database 


