#pragma once
#include <shared/versioning/Version.h>

/*
   DO NOT USE use directly this file
   
   Please use this line instead :
      shared::CServiceLocator::instance().get<IRunningInformation>().getSoftwareVersion()
   
*/
const shared::versioning::CVersion YadomsVersion("2.0.1-rc.1");

