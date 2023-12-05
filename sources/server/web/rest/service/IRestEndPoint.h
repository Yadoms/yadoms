#pragma once
#include <functional>
#include <string>
#include <shared/http/HttpRestVerb.h>

#include "web/rest/IRequest.h"
#include "web/rest/IAnswer.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         //----------------------------
         ///\brief   Interface for REST end-point
         //----------------------------
         class IRestEndPoint
         {
         public:
            virtual ~IRestEndPoint() = default;

            //--------------------------------------   
            ///\brief   Verb
            //-------------------------------------- 
            virtual shared::http::ERestVerb verb() const = 0;

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
            virtual std::function<boost::shared_ptr<IAnswer>(boost::shared_ptr<IRequest>)> handler() const = 0;
         };
      } //namespace service
   } //namespace rest
} //namespace web 
