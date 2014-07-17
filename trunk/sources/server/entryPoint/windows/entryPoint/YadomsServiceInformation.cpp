#include "stdafx.h"
#include "YadomsServiceInformation.h"

namespace entryPoint {
   const std::string CYadomsServiceInformation::ServiceName = "YadomsService";
   const std::string CYadomsServiceInformation::ServiceDisplayName = "Yadoms Service";
   const tools::service::CServiceInstaller::EServiceStartType CYadomsServiceInformation::StartType = tools::service::CServiceInstaller::kAutoStart;
   const std::string CYadomsServiceInformation::ServiceAccount = "NT AUTHORITY\\LocalService";
   const std::string CYadomsServiceInformation::ServiceAccountPassword = "";
   const std::string CYadomsServiceInformation::ServiceDependencies = "";
   
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
   
   const std::string & CYadomsServiceInformation::getServiceDisplayName()
   {
      return ServiceDisplayName;
   }
   
   const tools::service::CServiceInstaller::EServiceStartType  CYadomsServiceInformation::getStartType()
   {
      return StartType;
   }
   
   const std::string & CYadomsServiceInformation::getServiceAccount()
   {
      return ServiceAccount;
   }
   
   const std::string & CYadomsServiceInformation::getServiceAccountPassword()
   {
      return ServiceAccountPassword;
   }

   const std::string & CYadomsServiceInformation::getServiceDependencies()
   {
      return ServiceDependencies;
   }

   
} //namespace entryPoint
