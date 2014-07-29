#pragma once

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
   };

} //namespace service
} //namespace tools