#include "stdafx.h"
#include "Version1.h"
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
      return boost::shared_ptr<IVersionUpgrade>(new CVersion1()); 
   }

} //namespace versioning
} //namespace common
} //namespace database 

