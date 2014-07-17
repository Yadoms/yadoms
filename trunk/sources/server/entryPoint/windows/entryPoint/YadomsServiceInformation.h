#pragma once
#include "tools/service/IApplicationServiceInformation.h"

namespace entryPoint {

   //-----------------------------
   ///\brief   Class container for Yadoms service information
   ///         Provide information about WindowsService
   //-----------------------------   
   class CYadomsServiceInformation : public tools::service::IApplicationServiceInformation
   {
   public:
      //-----------------------------
      /// \brief Constructor
      //-----------------------------
      CYadomsServiceInformation();

      //-----------------------------
      /// \brief Destructor
      //-----------------------------
      virtual ~CYadomsServiceInformation();

      // IApplicationServiceInformation implementation
      virtual const std::string & getServiceName();
      virtual const std::string & getServiceDisplayName();
      virtual const tools::service::CServiceInstaller::EServiceStartType  getStartType();
      virtual const std::string & getServiceAccount();
      virtual const std::string & getServiceAccountPassword();
      virtual const std::string & getServiceDependencies();
      // [END] - IApplicationServiceInformation implementation

   private:
      //
      /// \brief The service name
      //
      static const std::string ServiceName;

      //
      /// \brief The service display name
      //
      static const std::string ServiceDisplayName;

      //
      /// \brief The service start type
      //
      static const tools::service::CServiceInstaller::EServiceStartType  StartType;

      //
      /// \brief The service dependencies
      //
      static const std::string ServiceDependencies;

      //
      /// \brief The service account
      //
      static const std::string ServiceAccount;

      //
      /// \brief The service account password
      //
      static const std::string ServiceAccountPassword;
   };

} //namespace entryPoint