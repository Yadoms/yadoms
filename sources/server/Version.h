#pragma once
#include <shared/versioning/Version.h>

/*
   DO NOT USE use directly this file
   
   Please use this line instead :
      shared::CServiceLocator::instance().get<IRunningInformation>().getSoftwareVersion()
   
   Version is taken from changelog.md

*/
const shared::versioning::CVersion YadomsVersion(YADOMS_VERSION);

