#pragma once

#include "rest/service/IRestService.h"

namespace web {

   //
   //\brief Interface for web server
   //
   class IRestHandler
   {
   public:
      virtual ~IRestHandler()
      {
      }

      //-----------------------------------------
      ///\brief   Initialize the REST services
      //-----------------------------------------
      virtual void initialize() = 0;

      //-----------------------------------------
      ///\brief      Register a REST service
      ///\param [in] restService the REST service to register
      //-----------------------------------------
      virtual void registerRestService(boost::shared_ptr<web::rest::service::IRestService> restService) =0;
   };

} //namespace web
