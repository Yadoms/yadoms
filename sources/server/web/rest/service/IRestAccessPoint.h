#pragma once
#include <functional>
#include <string>
#include "shared/http/HttpRestMethod.h"
#include "web/rest/ResultV2.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         //----------------------------
         ///\brief   Interface for REST access point
         //----------------------------
         class IRestAccessPoint
         {
         public:
            virtual ~IRestAccessPoint() = default;

            //--------------------------------------   
            ///\brief   Method
            //-------------------------------------- 
            virtual shared::http::ERestMethod method() const = 0;

            //--------------------------------------   
            ///\brief   Path (url part after "/rest/")
            //-------------------------------------- 
            virtual std::string path() const = 0;

            //--------------------------------------   
            ///\brief   Endpoint to call
            ///\note
            /// - First arg : map containing query params
            /// - Second arg : body
            /// Returns the answer
            //-------------------------------------- 
            virtual std::function<boost::shared_ptr<IRestAnswer>(std::map<std::string, std::string>, std::string)> handler() const = 0;
         };
      } //namespace service
   } //namespace rest
} //namespace web 
