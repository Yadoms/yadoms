#pragma once
#include "IRestAccessPoint.h"
#include "web/rest/RestDispatcher.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         //----------------------------
         ///\brief   Interface for REST service
         //----------------------------
         class IRestService
         {
         public:
            virtual ~IRestService() = default;

            //--------------------------------------   
            ///\brief   Method called to initialize the dispatcher (Poco-based webserver)
            //-------------------------------------- 
            virtual void configurePocoDispatcher(CRestDispatcher& dispatcher) = 0;

            //--------------------------------------   
            ///\brief   List all access points of the service (Oatpp-based webserver)
            //-------------------------------------- 
            virtual boost::shared_ptr<std::vector<boost::shared_ptr<IRestAccessPoint>>> accessPoints() = 0;
         };
      } //namespace service
   } //namespace rest
} //namespace web 
