#include "stdafx.h"
#include "Version_1_1_0.h"
#include "VersionUpgraderFactory.h"

namespace database { namespace common { namespace versioning { 

   CVersionUpgraderFactory::CVersionUpgraderFactory()
   {
   }

   CVersionUpgraderFactory::~CVersionUpgraderFactory()
   {
   }

   boost::shared_ptr<IVersionUpgrade> CVersionUpgraderFactory::GetUpgrader()
   { 
      //change this line when a new SQlite version is released
      return boost::make_shared<CVersion_1_1_0>();
   }

} //namespace versioning
} //namespace common
} //namespace database 

