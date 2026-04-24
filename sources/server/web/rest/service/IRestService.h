#pragma once
#include <vector>
#include "IRestEndPoint.h"
#include "web/poco/RestDispatcher.h"

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
            virtual void configurePocoDispatcher(poco::CRestDispatcher& dispatcher) = 0;

            //--------------------------------------   
            ///\brief   List all end-points of the service (Oatpp-based webserver)
            //-------------------------------------- 
            virtual boost::shared_ptr<std::vector<boost::shared_ptr<IRestEndPoint>>> endPoints() = 0;
         };
      } //namespace service
   } //namespace rest
} //namespace web 
