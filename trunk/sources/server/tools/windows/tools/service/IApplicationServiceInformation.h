#pragma once
#include "ServiceInstaller.h"

namespace tools { namespace service {

   //-----------------------------
   ///\brief   Interface for service information
   //-----------------------------   
   class IApplicationServiceInformation
   {
   public:
      //
      /// \brief The service name
      //
      virtual const std::string & getServiceName() = 0;

      //
      /// \brief The service display name
      //
      virtual const std::string & getServiceDisplayName() = 0;

      //
      /// \brief The service start type
      //
      virtual const service::CServiceInstaller::EServiceStartType  getStartType() = 0;

      //
      /// \brief The service dependencies
      //
      virtual const std::string & getServiceDependencies() = 0;
      
      //
      /// \brief The service account
      //
      virtual const std::string & getServiceAccount() = 0;
      
      //
      /// \brief The service account password
      //
      virtual const std::string & getServiceAccountPassword() = 0;
   };

} //namespace service
} //namespace tools