#include "stdafx.h"
#include "VersionUpgraderFactory.h"
#include "Version_5_2_0.h"

using namespace database::common::versioning;

boost::shared_ptr<IVersionUpgrade> CVersionUpgraderFactory::GetUpgrader()
{
   //change this line when a new database version is released
   return boost::make_shared<CVersion_5_2_0>(); //TODO passer en 6.0.0
}
