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
      
      const EUpdateType CYadoms::getUpdateType() const
      {
         return EUpdateType::kYadoms;
      }
  
   } // namespace source
} // namespace update
