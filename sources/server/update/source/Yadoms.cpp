#include "stdafx.h"
#include "Yadoms.h"
#include "IRunningInformation.h"
#include <shared/ServiceLocator.h>

namespace update {
   namespace source {

      CYadoms::CYadoms()
      {

      }

      CYadoms::~CYadoms()
      {

      }
      
      const std::string CYadoms::getUpdateLocation() const
      {
         return shared::CServiceLocator::instance().get<IRunningInformation>()->getExecutablePath();
      }

      const tools::CVersion CYadoms::getInstalledVersion() const
      {
         return shared::CServiceLocator::instance().get<IRunningInformation>()->getSoftwareVersion();
      }

      const EUpdateType CYadoms::getUpdateType() const
      {
         return EUpdateType::kYadoms;
      }
  
   } // namespace source
} // namespace update
