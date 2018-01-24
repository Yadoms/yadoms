#pragma once
#include <shared/versioning/Version.h>

/*
   DO NOT USE use directly this file
   
   Please use this line instead :
      shared::CServiceLocator::instance().get<IRunningInformation>().getSoftwareVersion()
   
*/

/*
   WHEN UPDATING THIS LINE, DON'T FORGET TO UPDATE changelog.md FILE
*/
const shared::versioning::CVersion YadomsVersion("2.0.2-beta.4");

