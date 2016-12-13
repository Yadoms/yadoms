#pragma once
#include <shared/versioning/ReleaseType.h>

/*
   DO NOT USE use directly this file
   
   Please use this line instead :
      shared::CServiceLocator::instance().get<IRunningInformation>().getSoftwareVersion()
   
*/
const std::string YadomsVersion("2.0.0.0");
const shared::versioning::EReleaseType YadomsReleaseType(shared::versioning::EReleaseType::kBeta);

