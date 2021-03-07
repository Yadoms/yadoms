#pragma once

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
            ///\brief   Method called to initialize the dispatcher
            //-------------------------------------- 
            virtual void configureDispatcher(CRestDispatcher& dispatcher) = 0;
         };
      } //namespace service
   } //namespace rest
} //namespace web 
