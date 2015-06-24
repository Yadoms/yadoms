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
      // [END] - IApplicationServiceInformation implementation

   private:
      //
      /// \brief The service name
      //
      static const std::string ServiceName;
   };

} //namespace entryPoint