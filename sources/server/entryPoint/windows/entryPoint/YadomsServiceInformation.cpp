#include "stdafx.h"
#include "YadomsServiceInformation.h"

namespace entryPoint {
   const std::string CYadomsServiceInformation::ServiceName = "Yadoms";
   
   CYadomsServiceInformation::CYadomsServiceInformation()
   {
   }
   
   CYadomsServiceInformation::~CYadomsServiceInformation()
   {
   }
   
   const std::string & CYadomsServiceInformation::getServiceName()
   {
      return ServiceName;
   }
   
   
} //namespace entryPoint
